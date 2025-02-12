#ifndef UTILS_JAVAUTILS_H_
#define UTILS_JAVAUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <regex>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace java
		{
			/// <summary>
			/// Returns the path to the executable. Returns None if none is found.
			/// </summary>
			/// <param name="jvm_version"></param>
			/// <param name="minecraft_directory"></param>
			/// <returns></returns>
			std::filesystem::path getExecutablePath(const std::string& jvm_version, const std::filesystem::path& minecraft_directory);

			/// <summary>
			/// Get the name that is used the identify the platform
			/// </summary>
			/// <returns></returns>
			std::string getJvmPlatformString();
		}
	}
}

#endif // !UTILS_JAVAUTILS_H_
