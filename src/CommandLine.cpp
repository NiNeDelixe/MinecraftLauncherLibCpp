#include "MinecraftLauncherLib/CommandLine.h"

std::vector<std::u32string> MCLCPPLIB_NAMESPACE::commandline::generateCommandLine(const UserProfile& user_options, const VersionProfile& version_options)
{
	/*
	Returns a command for launching Minecraft.For more information take a look at the documentation.
	*/
	VersionProfile version_profile_copy = version_options;
	std::filesystem::path minecraft_directory = version_profile_copy.getInstancePath();
	std::u32string version = version_profile_copy.getVersion();

	std::vector<std::u32string> command;

	if (!std::filesystem::exists(minecraft_directory / "versions" / version / (version + U".json")) ||
		!std::filesystem::is_directory(minecraft_directory / "versions" / version))
	{
		std::string error = "Version Not Found {1}";
		utils::string::replace(error, std::string("{1}"), version);
		throw std::invalid_argument(error);
	}
	
	nlohmann::json data;
	std::ifstream ifs(minecraft_directory / "versions" / version / (version + U".json"));
	ifs >> data;
	ifs.close();

	if (data.contains("inheritsFrom"))
	{
		data = utils::json::inheritJson(data, minecraft_directory);
	}
	if (version_profile_copy.getNativesPath().empty())
	{
		version_profile_copy.setNativesPath(minecraft_directory / "versions" / data["id"].template get<std::string>() / "natives");
	}

	version_profile_copy.setLibraries(Libraries::fromJson(data, minecraft_directory));

	// Add Java executable
	if (!version_profile_copy.getJavaPath().empty())
	{
		command.push_back(version_profile_copy.getJavaPath().u32string());
	}
	else if (data.contains("javaVersion"))
	{
		std::filesystem::path java_path = utils::java::getExecutablePath(data["javaVersion"]["component"].template get<std::string>(), minecraft_directory);
		if (java_path.empty())
		{
			command.push_back(U"java");
		}
		else
		{
			command.push_back(java_path.u32string());
		}
	}
	else
	{
		command.push_back(U"java"/*options.get("defaultExecutablePath", std::string("java"))*/);
	}

	if (!version_profile_copy.getJavaArguments().empty())
	{
		command.push_back(version_profile_copy.getJavaArguments().toU32String());
		//command.insert(command.end(), version_profile_copy.getJavaArguments().fromJson().begin(), version_profile_copy.getJavaArguments().end());
	}

	if (data["arguments"].type() == nlohmann::json::value_t::object)
	{
		if (data["arguments"].contains("jvm"))
		{
			auto argus = arguments::BasicArguments::fromJson(data["arguments"]["jvm"], data, minecraft_directory, user_options, version_profile_copy);
			command.push_back(argus.toU32String());
			//command.insert(command.end(), argus.begin(), argus.end());
		}
		else
		{
			command.push_back(U"-Djava.library.path=" + version_profile_copy.getNativesPath().u32string());
			command.push_back(U"-cp");
			command.push_back(version_profile_copy.getLibraries().toU32String());
		}
	}
	else
	{
		command.push_back(U"-Djava.library.path=" + version_profile_copy.getNativesPath().u32string());
		command.push_back(U"-cp");
		command.push_back(version_profile_copy.getLibraries().toU32String());
	}

	if (version_profile_copy.getUserOptions().enableLoggingConfig)
	{
		if (data.contains("logging"))
		{
			if (data["logging"].template get<int>() != 0)
			{
				std::filesystem::path logger_file = minecraft_directory / "assets" / "log_configs" / data["logging"]["client"]["file"]["id"].template get<std::string>();
				std::u32string data_replacer = data["logging"]["client"]["argument"].template get<std::u32string>();
				utils::string::replace(data_replacer, std::string("${path}"), logger_file.u32string());
				command.push_back(data_replacer);
			}
		}
	}

	command.push_back(data["mainClass"].template get<std::u32string>());

	if (data.contains("minecraftArguments"))
	{
		// For older versions
		command.push_back(arguments::BasicArguments::fromJson(data, minecraft_directory, user_options, version_profile_copy).toU32String());
	}
	else
	{
		auto args = arguments::BasicArguments::fromJson(data["arguments"]["game"], data, minecraft_directory, user_options, version_profile_copy);
		command.push_back(args.toU32String());
		//command.insert(command.end(), args.begin(), args.end());
	}

	if (!version_profile_copy.getUserOptions().server.empty())
	{
		command.push_back(U"--server");
		command.push_back(version_profile_copy.getUserOptions().server);

		if (!version_profile_copy.getUserOptions().port.empty())
		{
			command.push_back(U"--port");
			command.push_back(version_profile_copy.getUserOptions().port);
		}
	}

	return command;
}