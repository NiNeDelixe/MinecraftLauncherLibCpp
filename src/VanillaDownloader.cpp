#include "MinecraftLauncherLib/Download/VanillaDownloader.h"

MCLCPPLIB_NAMESPACE::download::VanillaDownloader::VanillaDownloader(const std::string& version_id,
	const std::filesystem::path& downloads_directory, const options::RuntimeOptions& options, const utils::delegates::callback_supplier& callback)
	: downloads_directory(downloads_directory), version_id(version_id), minecraft_path(options.minecraft_directory.get()),
	download_thread_pool("download-pool",
		[=]()
		{
			return std::make_shared<DownloadWorker>(downloads_directory);
		},
		[=](utils::download::DownloadResult& result)
		{
			this->results[result.file_name] = result;
			//results.push(result);
		}), 
	options(options),
	extraction_thread_pool("extraction-pool",
		[=]()
		{
			return std::make_shared<archives::ExtractorWorker>("");
		},
		[=](archives::ExtractResult& result)
		{
			//this->results[result.file_name] = result;
			//results.push(result);
		}), 
	callback(callback)
{
	download_thread_pool.setStandaloneResults(false);
	download_thread_pool.setStopOnFail(false);
	download_thread_pool.setOnComplete([=]() { extraction_thread_pool.update(); });

	//extraction_thread_pool.setStandaloneResults(false);
	extraction_thread_pool.setStopOnFail(false);
	extraction_thread_pool.deactivate();

	path_to_version_json =
		this->minecraft_path / "versions" / this->version_id / (this->version_id + ".json");
}

void MCLCPPLIB_NAMESPACE::download::VanillaDownloader::installVersion(const std::filesystem::path& url)
{
	if (url != "")
	{
		this->download_thread_pool.enqueueJobWithPriority(
			std::make_shared<DownloadWorker>(this->path_to_version_json, url, callback),
			this->path_to_version_json.filename().string());
		/*DownloadWorker w(logger_file, url, callback);
		w.download(); */
	}

	this->download_thread_pool.waitForPriorityJob(this->path_to_version_json.filename().string());
	std::ifstream ifstr(this->path_to_version_json);
	nlohmann::json version_data = nlohmann::json::parse(ifstr);
	ifstr.close();

	installLibraries(version_data);
	installAssets(version_data);

	if (version_data.contains("logging") && version_data["logging"].size() != 0)
	{
		std::filesystem::path logger_file = this->options.assets_directory.get() / "assets" / "log_configs" / version_data["logging"]["client"]["file"]["id"].template get<std::string>();
		std::filesystem::path url = version_data["logging"]["client"]["file"]["url"].template get<std::string>();

		this->download_thread_pool.enqueueJob(
			std::make_shared<DownloadWorker>(logger_file, url, callback));
		/*DownloadWorker w(logger_file, url, callback);
		w.download();*/
	}

	if (version_data.contains("downloads"))
	{
		std::filesystem::path file = this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / (version_data["id"].template get<std::string>() + ".jar");
		std::filesystem::path url = version_data["downloads"]["client"]["url"].template get<std::string>();
		std::string checksum = version_data["downloads"]["client"]["sha1"].template get<std::string>();

		this->download_thread_pool.enqueueJob(
			std::make_shared<DownloadWorker>(file, url, callback));

		/*DownloadWorker w(file, url, callback);
		w.download();*/
	}

	if (!std::filesystem::is_directory(this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / (version_data["id"].template get<std::string>() + ".jar")) && version_data.contains("inheritsFrom"))
	{
		std::filesystem::copy
		(
			this->minecraft_path / "versions" / version_data["inheritsFrom"].template get<std::string>() / (version_data["inheritsFrom"].template get<std::string>() + ".jar"),
			this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / (version_data["id"].template get<std::string>() + ".jar"),
			std::filesystem::copy_options::overwrite_existing
		);
	}


	//return utils::download::DownloadResult{"", CURLcode(0), this->minecraft_path};
}

bool MCLCPPLIB_NAMESPACE::download::VanillaDownloader::extractNativesFiles(const std::filesystem::path& filename, const std::filesystem::path& extract_path, const nlohmann::json& extract_data)
{
	if (!std::filesystem::create_directories(extract_path) && !std::filesystem::exists(extract_path)) 
	{
		return false;
	}

	if (!std::filesystem::exists(filename)) 
	{
		return false;
	}

	try 
	{
		utils::archives::Archive archive(filename.string());

		auto file_list = archive.getFileList();

		for (const auto& file_name : file_list) 
		{
			bool exclude = false;
			for (auto& var2 : extract_data["exclude"]) 
			{
				if (file_name.string().find(var2.get<std::string>()) == 0) 
				{
					exclude = true;
					break;
				}
			}
			if (exclude) 
			{
				continue;
			}

			if (file_name.string().back() == '/') 
			{
				continue;
			}

			std::filesystem::path output_path = extract_path / file_name;
			if (!std::filesystem::exists(output_path)) 
			{
				archive.extractFile(file_name, output_path);
			}
		}

	}
	catch (const std::exception& e) 
	{
		//std::cerr << "Error extracting archive: " << e.what() << std::endl;
		return false;
	}
}

void MCLCPPLIB_NAMESPACE::download::VanillaDownloader::doInstall()
{
	if (std::filesystem::exists(this->downloads_directory))
	{
		bool out_mkdir = std::filesystem::create_directories(this->downloads_directory);
	}

	if (std::filesystem::exists(this->path_to_version_json) && !std::filesystem::is_directory(this->path_to_version_json))
	{
		return installVersion();
	}

	std::filesystem::path manifest_url = "https://launchermeta.mojang.com/mc/game/version_manifest.json";

	{
		this->download_thread_pool.enqueueJobWithPriority(
			std::make_shared<DownloadWorker>(this->downloads_directory, manifest_url), manifest_url.filename().string());
	}

	/*DownloadWorker w(this->downloads_directory, manifest_url, callback);
	w.download();*/

	this->download_thread_pool.waitForPriorityJob(manifest_url.filename().string());

	std::fstream manifest_file(this->downloads_directory / manifest_url.filename());
	nlohmann::json json_manifest = nlohmann::json::parse(manifest_file);
	manifest_file.close();

	for (auto& var : json_manifest["versions"])
	{
		if (var["id"].template get<std::string>() == this->version_id)
		{
			return installVersion(var["url"].template get<std::string>());
		}
	}

	for (auto& var : json_manifest["versions"].items())
	{
		if (var.value()["id"].template get<std::string>() == this->version_id)
		{
			return installVersion(var.value()["url"].template get<std::string>());
		}
	}
}

void MCLCPPLIB_NAMESPACE::download::VanillaDownloader::installLoader()
{
	this->downloader_thread = std::make_shared<std::thread>([this]() { doInstall(); });
}

void MCLCPPLIB_NAMESPACE::download::VanillaDownloader::installLibraries(const nlohmann::json& version_data)
{
	int count = -1;
	for (auto& var : version_data["libraries"])
	{
		++count;

		// Check, if the rules allow this lib for the current system
		//TODO: add options
		if (!rules::Rules::parseRuleList(var, "rules"))
		{
			continue;
		}

		if (var.contains("downloads") && var["downloads"].contains("artifact"))
		{
			if (callback)
			{
				size_t size = var["downloads"]["artifact"]["size"].template get<int>();
				callback(nullptr, &size, nullptr, nullptr, nullptr);
			}
			std::filesystem::path url = var["downloads"]["artifact"]["url"].template get<std::string>();
			std::filesystem::path file = this->options.libraries_directory.get() / "libraries" / var["downloads"]["artifact"]["path"].template get<std::string>();
			std::string checksum = var["downloads"]["artifact"]["sha1"].template get<std::string>();
			download_thread_pool.enqueueJob(std::make_shared<DownloadWorker>(file, url, callback));
			continue;
		}

		// Turn the name into a path
		std::filesystem::path currentPath = this->options.libraries_directory.get() / "libraries";
		std::string downloadUrl = "";

		if (var.contains("url"))
		{
			if (*(--var["url"].template get<std::string>().end()) == '/')
			{
				
				std::string download_string = utils::string::rtrim_copy(var["url"].template get<std::string>(), '/');
				downloadUrl = download_string + "/";
			}
			else
			{
				downloadUrl = var["url"].template get<std::string>();
			}
		}
		else
		{
			downloadUrl = "https://libraries.minecraft.net/";
		}

		auto library = libraries::SingleLibrary::getLibraryFromName(var["name"].template get<std::string>(), this->options.libraries_directory.get());

		downloadUrl += library.getLibraryUrl().string();
		auto& lib_path = library.getLibraryPath();

		if (!std::filesystem::exists(lib_path))
		{
			//Try to download the lib
			download_thread_pool.enqueueJob(std::make_shared<DownloadWorker>(lib_path, downloadUrl, callback));
		}

		std::string native = libraries::Libraries::getNatives(var);

		std::string jar_filename_native;
		if (!native.empty())
		{
			jar_filename_native = library.getLibraryName() + "-" + library.getLibraryVersion() + "-" + native + ".jar";
		}

		if (!var.contains("downloads"))
		{
			if (var.contains("extract"))
			{
				std::filesystem::path extract_path = this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / "natives";
				if (!this->options.natives_directory.isDefault())
				{
					extract_path = this->options.natives_directory.get() / "natives";
				}
				std::filesystem::path archive = currentPath / jar_filename_native;
				this->extraction_thread_pool.enqueueJob(std::make_shared<utils::delegates::runnable>([=]() 
					{
						extractNativesFiles(archive, extract_path, var["extract"]);
					}));
				
			}
			continue;
		}
		if (native != "")
		{
			if (callback)
			{
				size_t size = var["downloads"]["classifiers"][native]["size"].template get<int>();
				callback(nullptr, &size, nullptr, nullptr, nullptr);
			}

			std::filesystem::path url = var["downloads"]["classifiers"][native]["url"].template get<std::string>();
			std::filesystem::path file = currentPath / jar_filename_native;
			std::filesystem::path checksum = var["downloads"]["artifact"]["sha1"].template get<std::string>();
			
			download_thread_pool.enqueueJob(std::make_shared<DownloadWorker>(file, url, callback));

			if (var["extract"] != nullptr)
			{
				std::filesystem::path natives_path = this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / "natives";
				if (!this->options.natives_directory.isDefault())
				{
					natives_path = this->options.natives_directory.get() / "natives";
				}
			
				this->extraction_thread_pool.enqueueJob(std::make_shared<utils::delegates::runnable>([=]()
					{
						extractNativesFiles(file, natives_path, var["extract"]);
					}));
			}
		}
		//callback->OnProgress(count, NULL, NULL, NULL);
	}

	this->extraction_thread_pool.activate();
}

std::string MCLCPPLIB_NAMESPACE::download::VanillaDownloader::getLatestInstallerVersion()
{
	return "";
}

void MCLCPPLIB_NAMESPACE::download::VanillaDownloader::installAssets(const nlohmann::json& version_data)
{
	/*
	Install all assets
	*/
	// Old versions dosen't have this
	if (!version_data.contains("assetIndex"))
	{
		return;
	}
	//callback->OnProgress(NULL, NULL, NULL, L"Download Assets");

	// Download all assets
	//callback->setTotalDownloadSize(version_data["assetIndex"]["size"].to_int());

	{
		std::filesystem::path url = version_data["assetIndex"]["url"].template get<std::string>();
		std::filesystem::path dw_path = this->options.assets_directory.get() / "assets" / "indexes" /
			(version_data["assets"].template get<std::string>() + ".json");

		this->download_thread_pool.enqueueJobWithPriority(std::make_shared<DownloadWorker>(dw_path, url), "assets_json");
	}

	/*DownloadFile(
		data["assetIndex"]["url"].to_string(),
		Join({ path, "assets", "indexes", (data["assets"].to_string() + ".json") }),
		callback,
		data["assetIndex"]["sha1"].to_string()
	);*/

	this->download_thread_pool.waitForPriorityJob("assets_json");

	std::ifstream file(this->minecraft_path / "assets" / "indexes" / (version_data["assets"].template get<std::string>() + ".json"));
	nlohmann::json assets_data = nlohmann::json::parse(file);

	// The assets has a hash. e.g. c4dbabc820f04ba685694c63359429b22e3a62b5
	// With this hash, it can be download from https://resources.download.minecraft.net/c4/c4dbabc820f04ba685694c63359429b22e3a62b5
	// And saved at assets/objects/c4/c4dbabc820f04ba685694c63359429b22e3a62b5
	 
	//callback->OnProgress(NULL, assets_data["objects"].get_count() - 1, NULL, NULL);

	int count = 0;
	for (auto& [key, value] : assets_data["objects"].items())
	{
		//callback->setTotalDownloadSize(value["size"].to_int());

		std::filesystem::path url = std::filesystem::path("https://resources.download.minecraft.net") /
			value["hash"].template get<std::string>().substr(0, 2) /
			value["hash"].template get<std::string>();

		std::filesystem::path dw_path = this->options.assets_directory.get()
			/ "assets" / "objects" /
			value["hash"].template get<std::string>().substr(0, 2) / value["hash"].template get<std::string>();

		this->download_thread_pool.enqueueJob(std::make_shared<DownloadWorker>(dw_path, url, this->callback));

		++count;
		//callback->OnProgress(count, NULL, NULL, NULL);
	}
	//callback->OnProgress(NULL, NULL, 6, NULL);

}

//auto mess = "";
//auto lvl = MESSEGE;
//size_t size = var["downloads"]["artifact"]["size"].template get<int>();
//callback(nullptr, &size, nullptr, &mess, &lvl);