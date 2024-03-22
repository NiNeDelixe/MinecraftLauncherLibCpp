#ifndef UTILS_PATHUTILS_H_
#define UTILS_PATHUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>

#include "MinecraftLauncherLib/Utils/StringUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		std::filesystem::path expandUser(const std::filesystem::path& path);
	}
}

#endif // !UTILS_PATHUTILS_H_