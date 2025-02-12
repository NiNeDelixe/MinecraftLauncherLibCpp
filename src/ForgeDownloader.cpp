#include "MinecraftLauncherLib/Download/ForgeDownloader.h"

void MCLCPPLIB_NAMESPACE::download::loaders::ForgeDownloader::doInstall()
{
	std::filesystem::path FORGE_DOWNLOAD_URL = "https://maven.minecraftforge.net/net/minecraftforge/forge/" + forge_version_comb + "/forge-" + forge_version_comb + "-installer.jar";
	std::filesystem::path temp_file_path = std::filesystem::temp_directory_path();

	{
		this->download_thread_pool.enqueueJobWithPriority(
			std::make_shared<DownloadWorker>(temp_file_path, FORGE_DOWNLOAD_URL), FORGE_DOWNLOAD_URL.filename().string());
	}
	/*DownloadWorker w(temp_file_path, FORGE_DOWNLOAD_URL);
	w.download();*/

	this->download_thread_pool.waitForPriorityJob(FORGE_DOWNLOAD_URL.filename().string());

	utils::archives::Archive archive(temp_file_path / FORGE_DOWNLOAD_URL.filename());

	nlohmann::json version_data = nlohmann::json::parse(archive.getBytesFromFile("install_profile.json"));

	std::string forge_version_id = version_data.contains("version") ?
		version_data["version"].template get<std::string>() :
		version_data["install"]["version"].template get<std::string>();
	std::string minecraft_version = version_data.contains("minecraft") ?
		version_data["minecraft"].template get<std::string>() :
		version_data["install"]["minecraft"].template get<std::string>();

	VanillaDownloader::installLoader();

	if (version_data.contains("libraries"))
	{
		installLibraries(version_data);
	}

	std::filesystem::path version_json_path = this->minecraft_path / "versions" / forge_version_id / (forge_version_id + ".json");
	try
	{
		archive.extractFile("version.json", version_json_path);
		if (version_data.contains("versionInfo"))
		{
			std::ofstream o(version_json_path);
			o << version_data["versionInfo"].dump(4) << std::endl;
			o.close();
		}
	}
	catch (const std::exception& ex)
	{
	}

	std::filesystem::path forge_lib_path =
		this->options.libraries_directory.get() / "libraries" / "net" / "minecraftforge" / "forge" / this->version_id;

	try
	{
		archive.extractFile("maven/net/minecraftforge/forge/" + this->version_id + "/forge-" + this->version_id + ".jar",
			forge_lib_path / ("forge-" + this->version_id + ".jar"));
		archive.extractFile("maven/net/minecraftforge/forge/" + this->version_id + "/forge-" + this->version_id + "-universal.jar",
			forge_lib_path / ("forge-" + this->version_id + "-universal.jar"));
	}
	catch (const std::exception& ex)
	{
	}

	try
	{
		archive.extractFile("forge-" + this->version_id + "-universal.jar", forge_lib_path / ("forge-" + this->version_id + ".jar"));
	}
	catch (const std::exception& ex)
	{
	}


	std::filesystem::path lzma_path = std::filesystem::temp_directory_path() / "forge-lzma.tmp";
	archive.extractFile("data/client.lzma", lzma_path);

	this->version_id = forge_version_id;
	this->path_to_version_json = this->minecraft_path / "versions" / this->version_id / (this->version_id + ".json");

	VanillaDownloader::installLoader();

	if (version_data.contains("processors"))
	{
		forgeProcessors(version_data, lzma_path, temp_file_path);
	}

	//DeleteFileW(temp_file_path.wstring().c_str());
	if (std::filesystem::exists(lzma_path) && !std::filesystem::is_directory(lzma_path))
	{
		std::filesystem::permissions(lzma_path, std::filesystem::perms::all);
		std::filesystem::remove(lzma_path);
	}
}

//void MCLCPPLIB_NAMESPACE::download::loaders::ForgeDownloader::installLoader()
//{
//	return doInstall();
//}

void MCLCPPLIB_NAMESPACE::download::loaders::ForgeDownloader::forgeProcessors(nlohmann::json& version_data, 
	const std::filesystem::path& lzma_path, const std::filesystem::path& installer_path)
{
	std::filesystem::path minecraft_path = this->minecraft_path;
	std::filesystem::path libraries_path = this->options.libraries_directory.get();

	nlohmann::json argument_vars = nlohmann::json(nlohmann::json::value_t::object);
	nlohmann::json value_ = nlohmann::json(minecraft_path / "versions" / version_data["minecraft"].template get<std::string>() /
		(version_data["minecraft"].template get<std::string>() + ".jar"));

	argument_vars["{MINECRAFT_JAR}"] = value_;

	for (auto& var : version_data["data"].items())
	{
		std::string client = var.value()["client"].template get<std::string>();
		if (client._Starts_with("[") && (*(--client.end()) == ']'))
		{
			std::string _libname = client.substr(1, client.size() - 2);
			auto lib = libraries::SingleLibrary::getLibraryFromName(_libname, libraries_path);
			argument_vars["{" + var.key() + "}"] = lib.getLibraryPath();
		}
		else
		{
			argument_vars["{" + var.key() + "}"] = client;
		}
	}

	std::filesystem::path root_path = std::filesystem::temp_directory_path() / "forge-root.tmp";

	if (argument_vars.contains("{INSTALLER}"))
	{
		argument_vars.erase("{INSTALLER}");
	}
	argument_vars["{INSTALLER}"] = installer_path;
	if (argument_vars.contains("{BINPATCH}"))
	{
		argument_vars.erase("{BINPATCH}");
	}
	argument_vars["{BINPATCH}"] = lzma_path;
	if (argument_vars.contains("{ROOT}"))
	{
		argument_vars.erase("{ROOT}");
	}
	argument_vars["{ROOT}"] = root_path;
	if (argument_vars.contains("{SIDE}"))
	{
		argument_vars.erase("{SIDE}");
	}
	argument_vars["{SIDE}"] = "client";

	int count = -1;
	for (auto& var : version_data["processors"])
	{
		++count;

		if (var["sides"] != nullptr)
		{
			if (var["sides"][0].template get<std::string>() != "client")
			{
				// Skip server side only processors
				continue;
			}
		}

		// Get the classpath
		libraries::Libraries classpath(minecraft_path);
		for (auto& var2 : var["classpath"])
		{
			classpath.add(
				libraries::SingleLibrary::getLibraryFromName(var2.template get<std::string>(),
					libraries_path));
		}

		classpath.add(libraries::SingleLibrary::getLibraryFromName(var["jar"].template get<std::string>(),
			libraries_path));

		this->download_thread_pool.join();

		std::string mainclass = java::Java::template getJarMainclass<std::string>(
			libraries::SingleLibrary::getLibraryFromName(var["jar"].template get<std::string>(), libraries_path).getLibraryPath());
		command::Command command;
		command.setJavaExcecutable(this->options.java_executable_path.get().empty() ? U"javaw" : this->options.java_executable_path.get().u32string());
		command.setMCLibraries(classpath);
		command.setJavaClassName(mainclass);

		std::u32string variable;
		for (auto& var2 : var["args"])
		{

			variable = argument_vars[var2.template get<std::string>()] == nullptr ?
				var2.template get<std::filesystem::path>().u32string() : 
				argument_vars[var2.template get<std::string>()].template get<std::filesystem::path>().u32string();

			if (variable._Starts_with(U"[") && (*(--variable.end()) == U']'))
			{
				command.addUserArgument(utils::arguments::Argument(libraries::SingleLibrary::getLibraryFromName(variable.substr(1, variable.size() - 2),
					libraries_path).getLibraryPath().u32string(), {}));
			}
			else
			{
				command.addUserArgument(utils::arguments::Argument(variable, {}));
			}
		}

		std::filesystem::path log_file = minecraft_path / "mc_lib";
		std::filesystem::create_directories(log_file);
		std::ofstream out_file(log_file / (mainclass + "_latest_out.txt"));
		options::ProcessOptions opt({ 0, &out_file, &out_file });
		utils::java::startJava(command, opt);

	}
	if (std::filesystem::exists(root_path))
	{
		std::filesystem::remove(root_path);
	}
}
