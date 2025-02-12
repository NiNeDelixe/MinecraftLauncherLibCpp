#include "MinecraftLauncherLib/CommandLine.h"

std::vector<std::u32string> MCLCPPLIB_NAMESPACE::command::vector::generateCommand(
    const MCLCPPLIB_NAMESPACE::VersionProfile& version_profile, const MCLCPPLIB_NAMESPACE::UserProfile& user_profile)
{
	/*
	Returns a command for launching Minecraft.For more information take a look at the documentation.
	*/
	MCLCPPLIB_NAMESPACE::VersionProfile game_profile = version_profile;
	std::filesystem::path minecraft_directory = game_profile.getInstancePath();
	std::u32string version = game_profile.getVersion();
	auto& options = user_profile.getOptions();

	auto& executable_path = options.java_executable_path.get();
	
	auto& data = game_profile.getJsonData();

	std::vector<std::u32string> command;

	// Add Java executable
	if (!executable_path.empty())
	{
		command.push_back(executable_path.u32string());
	}
	else if (data.contains("javaVersion"))
	{
		std::filesystem::path java_path = java::Java::getExecutablePath(data["javaVersion"]["component"].template get<std::string>(), minecraft_directory);
		if (java_path == "")
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
		command.push_back(options.java_executable_path.get().empty() ? std::u32string(U"java") : options.java_executable_path.get().u32string());
	}

	auto jvm_arguments = game_profile.getJavaArgs().toU32StringsVector();
	if (!jvm_arguments.empty())
	{
		command.insert(command.end(), jvm_arguments.begin(), jvm_arguments.end());
	}

	// Newer Versions have jvmArguments in version.json
	if (data["arguments"].type() == nlohmann::json::value_t::object)
	{
		if (data["arguments"].contains("jvm"))
		{
			auto argus = arguments::Arguments::getArgumentsFromJson(data["arguments"]["jvm"], data, user_profile, game_profile.getLibrariesPaths()).toU32StringsVector();
			command.insert(command.end(), argus.begin(), argus.end());
		}
		else
		{
			command.push_back(U"-Djava.library.path=" + options.natives_directory.get().u32string());
			command.push_back(U"-cp");
			command.push_back(libraries::Libraries::pathsToString(game_profile.getLibrariesPaths()));
		}
		
	}
	else
	{
		command.push_back(U"-Djava.library.path=" + options.natives_directory.get().u32string());
		command.push_back(U"-cp");
		command.push_back(libraries::Libraries::pathsToString(game_profile.getLibrariesPaths()));
	}

	/*if (options.get("enableLoggingConfig", false))
	{
		if (data.contains("logging"))
		{
			if (data["logging"].template get<int>() != 0)
			{
				std::filesystem::path logger_file = minecraft_directory / "assets" / "log_configs" / data["logging"]["client"]["file"]["id"].template get<std::string>();
				std::u32string data_replacer = data["logging"]["client"]["argument"].template get<std::u32string>();
				utils::string::replace(data_replacer, std::u32string(U"${path}"), logger_file.u32string());
				command.push_back(data_replacer);
			}
		}
	}*/

	command.push_back(game_profile.getJavaMainclass());

	if (data.contains("minecraftArguments"))
	{
		// For older versions
		//command.push_back(MinecraftCpp::QMinecraft::get_arguments_string(data, minecraft_directory, options));
	}
	else
	{
		auto args = arguments::Arguments::getArgumentsFromJson(data["arguments"]["game"], data, user_profile, game_profile.getLibrariesPaths()).toU32StringsVector();
		command.insert(command.end(), args.begin(), args.end());
	}

	/*if (!options.server.get().empty())
	{
		command.push_back(U"--server");
		command.push_back(options.server.get());

		if (!options.port.get().empty())
		{
			command.push_back(U"--port");
			command.push_back(options.port.get());
		}
	}*/

	return command;
}

std::u32string MCLCPPLIB_NAMESPACE::command::string::generateCommand(
    const MCLCPPLIB_NAMESPACE::VersionProfile& game_profile, const MCLCPPLIB_NAMESPACE::UserProfile& user_profile)
{
	auto command = vector::generateCommand(game_profile, user_profile);

	std::u32string s;
	for (const auto& piece : command) 
		s += piece + U" ";
	return s;
}
