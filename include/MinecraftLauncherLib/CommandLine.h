#ifndef MINECRAFTLAUNCHERLIB_COMMANDLINE_H_
#define MINECRAFTLAUNCHERLIB_COMMANDLINE_H_

#include "MCLLib-api.h"

#include <vector>
#include <string>

#include "MinecraftLauncherLib/Utils/JsonUtils.h"
#include "MinecraftLauncherLib/Utils/JavaUtils.h"

#include "MinecraftLauncherLib/UserProfile.h"
#include "MinecraftLauncherLib/VersionProfile.h"
#include "MinecraftLauncherLib/Arguments.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace commandline
	{
		std::vector<std::u32string> generateCommandLine(const MCLCPPLIB_NAMESPACE::UserProfile& user_options, const MCLCPPLIB_NAMESPACE::VersionProfile& version_options);
	}
}

#endif // !MINECRAFTLAUNCHERLIB_COMMANDLINE_H_