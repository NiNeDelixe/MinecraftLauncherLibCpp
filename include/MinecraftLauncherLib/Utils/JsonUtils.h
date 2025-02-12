#ifndef UTILS_JSONUTILS_H_
#define UTILS_JSONUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace json
		{
			/// <summary>
			/// Implement the inheritsFrom function
			/// <para>See https://github.com/tomsik68/mclauncher-api/wiki/Version-Inheritance-%26-forge/</para> 
			/// </summary>
			/// <param name="original_data">data from vanilla json file</param>
			/// <param name="minecraft_path">minecraft root path</param>
			/// <returns></returns>
			nlohmann::json inheritJson(const nlohmann::json& original_data, const std::filesystem::path& minecraft_path);
		}
	}
}

#endif // !UTILS_JSONUTILS_H_
