#ifndef MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
#define MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/JavaArguments.h"
#include "MinecraftLauncherLib/UserArguments.h"
#include "MinecraftLauncherLib/UserProfile.h"
#include "MinecraftLauncherLib/Libraries.h"

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	class VersionProfile
	{
	public:
		VersionProfile(const std::string& version, const options::RuntimeOptions& options);
		~VersionProfile() = default;

	public:
		void fromJson(const std::filesystem::path& json_path, const options::RuntimeOptions& options);

	public:
		static nlohmann::json inheritJson(const nlohmann::json& original_data, const std::filesystem::path& path);

	public:
		// Getters
		std::u32string getVersion() const { return version; }
		std::filesystem::path getInstancePath() const { return instance_path; }
		std::filesystem::path getJavaPath() const { return java_path; }
		arguments::JavaArguments getJavaArgs() const { return java_args; }
		std::filesystem::path getNativesPath() const { return natives_path; }
		types::Vector<std::filesystem::path> getLibrariesPaths() const { return libraries_paths; }
		std::u32string getJavaMainclass() const { return java_mainclass; }
		arguments::UserArguments getUserArguments() const { return user_arguments; }
		std::u32string getAssetsIndexId() const { return assets_index_id; }

		// Setters
		void setVersion(const std::u32string& version) { this->version = version; }
		void setInstancePath(const std::filesystem::path& instance_path) { this->instance_path = instance_path; }
		void setJavaPath(const std::filesystem::path& java_path) { this->java_path = java_path; }
		void setJavaArgs(const arguments::JavaArguments& java_args) { this->java_args = java_args; }
		void setNativesPath(const std::filesystem::path& natives_path) { this->natives_path = natives_path; }
		void setLibrariesPaths(const types::Vector<std::filesystem::path>& libraries_paths) { this->libraries_paths = libraries_paths; }
		void setJavaMainclass(const std::u32string& java_mainclass) { this->java_mainclass = java_mainclass; }
		void setUserArguments(const arguments::UserArguments& user_arguments) { this->user_arguments = user_arguments; }
		void setAssetsIndexId(const std::u32string& assets_index_id) { this->assets_index_id = assets_index_id; }

	private:
		std::u32string version; //TODO: can be string
		std::filesystem::path instance_path;
		
		std::filesystem::path java_path;
		arguments::JavaArguments java_args;
		std::filesystem::path natives_path;
		types::Vector<std::filesystem::path> libraries_paths;

		std::u32string java_mainclass; //TODO: can be string
		arguments::UserArguments user_arguments;
		std::u32string assets_index_id;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
