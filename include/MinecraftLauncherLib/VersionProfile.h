#ifndef MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
#define MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "UserProfile.h"
#include "JavaArguments.h"
#include "UserArguments.h"

#include "Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	class VersionProfile
	{
	public:
		VersionProfile(const std::filesystem::path& instance_path);
		~VersionProfile() = default;

	public:
		void fromJson(const std::filesystem::path& json_path);

	private:
		std::u32string version;
		std::filesystem::path instance_path;
		
		std::filesystem::path java_path;
		arguments::JavaArguments java_args;
		std::filesystem::path natives_path;
		types::Vector<std::filesystem::path> libraries_paths;

		std::u32string java_mainclass;
		arguments::UserArguments user_arguments;
		std::u32string assets_index_id;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_VERSIONPROFILE_H_
