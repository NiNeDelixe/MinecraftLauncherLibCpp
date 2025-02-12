#ifndef MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
#define MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/JsonUtils.h"

#include "MinecraftLauncherLib/UserProfile.h"
#include "MinecraftLauncherLib/JavaArguments.h"
#include "MinecraftLauncherLib/UserArguments.h"
#include "MinecraftLauncherLib/Basic/MinecraftObject.h"
#include "MinecraftLauncherLib/Libraries.h"

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	class VersionProfile
	{
	public:
		struct UserOptions
		{
			int resolutionHeight = 480;
			int resolutionWidth = 854;
			bool demo = false;
			bool customResolution = false;
			bool enablePrettyPathOutput = false;
			bool enableLoggingConfig = false;
			std::u32string server = U"";
			std::u32string port = U"";
		};

	public:
		VersionProfile(const std::filesystem::path& instance_path, const std::u32string& version)
			: instance_path(instance_path), version(version) {}
		VersionProfile() = default;
		~VersionProfile() = default;

	public:
		void fromJson(const std::filesystem::path& json_path)
		{
			std::filesystem::path json_path_local = json_path;
			if (this->user_options.enablePrettyPathOutput)
			{
				json_path_local = json_path_local.lexically_normal();
			}

			std::ifstream ifstr(json_path_local);
			nlohmann::json version_json = nlohmann::json::parse(ifstr);
			ifstr.close();

			if (this->instance_path.empty())
			{
				this->instance_path = json_path_local.parent_path().parent_path().parent_path();
			}

			if (version_json.contains("inheritsFrom"))
			{
				version_json = utils::json::inheritJson(version_json, this->instance_path);
			}

			if (this->natives_path.empty())
			{
				this->natives_path = this->instance_path / "versions" / version_json["id"].template get<std::string>() / "natives";
			}

			this->libraries = MCLCPPLIB_NAMESPACE::Libraries::fromJson(version_json, this->instance_path);
		}
		static VersionProfile fromJson(const std::filesystem::path& minecraft_directory, const std::u32string& version, const UserOptions& options)
		{
			VersionProfile profile(minecraft_directory, version);
			profile.setUserOptions(options);
			profile.fromJson(minecraft_directory / "versions" / version / (version + U".json"));
			return profile;
		}

	public:
		const std::u32string& getVersion() const { return this->version; }
		const std::filesystem::path& getInstancePath() const { return this->instance_path; }
		const std::filesystem::path& getJavaPath() const { return this->java_path; }
		const arguments::JavaArguments& getJavaArguments() const { return this->java_args; }
		const std::filesystem::path& getNativesPath() const { return this->natives_path; }
		const Libraries& getLibraries() const { return this->libraries; }
		const std::u32string& getJavaMainclass() const { return this->java_mainclass; }
		const arguments::UserArguments& getUserArguments() const { return this->user_arguments; }
		const std::u32string& getAssetsIndexId() const { return this->assets_index_id; }
		const UserOptions& getUserOptions() const { return this->user_options; }

	public:
		void setVersion(const std::u32string& value) { this->version = value; }
		void setInstancePath(const std::filesystem::path& value) { this->instance_path = value; }
		void setJavaPath(const std::filesystem::path& value) { this->java_path = value; }
		void setJavaArguments(const arguments::JavaArguments& value) { this->java_args = value; }
		void setNativesPath(const std::filesystem::path& value) { this->natives_path = value; }
		void setLibraries(const Libraries& value) { this->libraries = value; }
		void setJavaMainclass(const std::u32string& value) { this->java_mainclass = value; }
		void setUserArguments(const arguments::UserArguments& value) { this->user_arguments = value; }
		void setAssetsIndexId(const std::u32string& value) { this->assets_index_id = value; }
		void setUserOptions(const UserOptions& value) { this->user_options = value; }

	private:
		std::u32string version = U"";
		std::filesystem::path instance_path = "";
		
		std::filesystem::path java_path = "";
		arguments::JavaArguments java_args = arguments::JavaArguments();
		std::filesystem::path natives_path = "";
		Libraries libraries = Libraries();

		std::u32string java_mainclass = U"";
		arguments::UserArguments user_arguments = arguments::UserArguments();
		std::u32string assets_index_id = U"";

		UserOptions user_options = UserOptions();
	};
}

#endif // !MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
