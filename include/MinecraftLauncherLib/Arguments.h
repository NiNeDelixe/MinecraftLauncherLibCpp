#ifndef MINECRAFTLAUNCHERLIB_ARGUMENTS_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENTS_H_

#include "MCLLib-api.h"

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/StringUtils.h"
#include "MinecraftLauncherLib/Utils/Macros.h"

#include "MinecraftLauncherLib/Argument.h"
#include "MinecraftLauncherLib/UserProfile.h"
//#include "MinecraftLauncherLib/VersionProfile.h"
#include "MinecraftLauncherLib/Rules.h"


namespace MCLCPPLIB_NAMESPACE
{
	class VersionProfile;

	namespace arguments
	{
		class BasicArguments
		{
		public:
			static arguments::BasicArguments fromJson(const nlohmann::json& data, const nlohmann::json& version_data,
				const std::filesystem::path& minecraft_path, const UserProfile& user_options, const VersionProfile& profile_options);

			static arguments::BasicArguments fromJson(const nlohmann::json& version_data, const std::filesystem::path& minecraft_path,
				const UserProfile& user_options, const VersionProfile& profile_options);

			template<typename STR>
			static STR& replaceArgument(STR& argstr, const nlohmann::json& versionData,
				const std::filesystem::path& path, const MCLCPPLIB_NAMESPACE::UserProfile& user_options, const MCLCPPLIB_NAMESPACE::VersionProfile& profile_options)
			{
				utils::string::replace(argstr, std::wstring(L"${natives_directory}"), profile_options.getNativesPath().wstring());
				utils::string::replace(argstr, std::wstring(L"${launcher_name}"), std::string("null")/*options.get("launcherName", std::string("null"))*/);
				utils::string::replace(argstr, std::string("${launcher_version}"), std::string("null")/*options.get("launcherVersion", std::string("null"))*/);
				utils::string::replace(argstr, std::u32string(U"${classpath}"), profile_options.getLibraries().toU32String());
				utils::string::replace(argstr, std::string("${auth_player_name}"), !user_options.getUsername().empty() ? user_options.getUsername() : std::u32string(U"{username}"));
				utils::string::replace(argstr, std::string("${version_name}"), versionData["id"].template get<std::string>());
				utils::string::replace(argstr, std::string("${game_directory}"), !profile_options.getInstancePath().empty() ? profile_options.getInstancePath().wstring() : path.wstring());
				utils::string::replace(argstr, std::string("${assets_root}"), path / "assets");
				utils::string::replace(argstr, std::string("${assets_index_name}"), !versionData["assets"].empty() ? versionData["assets"].template get<std::string>() : versionData["id"].template get<std::string>());
				utils::string::replace(argstr, std::string("${auth_uuid}"), user_options.getUUID().empty() ? std::u32string(U"{uuid}") : user_options.getUUID());
				utils::string::replace(argstr, std::string("${auth_access_token}"), user_options.getAccessToken().empty() ? std::u32string(U"{token}") : user_options.getAccessToken());
				utils::string::replace(argstr, std::string("${user_type}"), std::string("mojang"));
				utils::string::replace(argstr, std::string("${version_type}"), versionData["type"].template get<std::string>());
				utils::string::replace(argstr, std::string("${user_properties}"), std::string("{}"));
				utils::string::replace(argstr, std::string("${resolution_width}"), std::string("854")/*options.get("resolutionWidth", std::string("854"))*/);
				utils::string::replace(argstr, std::string("${resolution_height}"), std::string("480")/*options.get("resolutionHeight", std::string("480"))*/);
				utils::string::replace(argstr, std::wstring(L"${game_assets}"), (path / "assets" / "virtual" / "legacy").wstring());
				utils::string::replace(argstr, std::string("${auth_session}"), user_options.getAccessToken().empty() ? std::u32string(U"{token}") : user_options.getAccessToken());
				utils::string::replace(argstr, std::wstring(L"${library_directory}"), (path / "libraries").wstring());
				utils::string::replace(argstr, std::string("${classpath_separator}"), std::string(1, CLASSPATH_SEPARATOR));
				utils::string::replace(argstr, std::string("${quickPlayPath}"), std::string("quickPlayPath")/*options.get("quickPlayPath", std::string("quickPlayPath"))*/);
				utils::string::replace(argstr, std::string("${quickPlaySingleplayer}"), std::string("quickPlaySingleplayer")/*options.get("quickPlaySingleplayer", std::string("quickPlaySingleplayer"))*/);
				utils::string::replace(argstr, std::string("${quickPlayMultiplayer}"), std::string("quickPlayMultiplayer")/*options.get("quickPlayMultiplayer", std::string("quickPlayMultiplayer"))*/);
				utils::string::replace(argstr, std::string("${quickPlayRealms}"), std::string("quickPlayRealms")/*options.get("quickPlayRealms", std::string("quickPlayRealms"))*/);

				return argstr;
			}

		public:
			std::u32string toU32String() const
			{
				std::u32string string;

				for (auto& elem : arguments)
				{
					string += elem.toU32String() + U" ";
				}

				return string;
			}

			bool empty() const { return this->arguments.empty(); }

		public:
			const types::Vector<utils::arguments::Argument>& getArguments() const { return this->arguments; }

		protected:
			types::Vector<utils::arguments::Argument> arguments = types::Vector<utils::arguments::Argument>();
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENTS_H_