#ifndef MINECRAFTLAUNCHERLIB_COMMANDLINE_H_
#define MINECRAFTLAUNCHERLIB_COMMANDLINE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <vector>

#include "MinecraftLauncherLib/VersionProfile.h"
#include "MinecraftLauncherLib/UserProfile.h"
#include "MinecraftLauncherLib/Core/Libraries.h"
#include "MinecraftLauncherLib/Core/Java.h"
#include "MinecraftLauncherLib/Core/Arguments.h"
#include "MinecraftLauncherLib/Utils/StringUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace command
	{
		namespace vector
		{
			std::vector<std::u32string> MCLLIB_API generateCommand(const MCLCPPLIB_NAMESPACE::VersionProfile& game_profile,
				const MCLCPPLIB_NAMESPACE::UserProfile& user_profile);
		}
		namespace string
		{
			std::u32string MCLLIB_API generateCommand(const MCLCPPLIB_NAMESPACE::VersionProfile& game_profile,
				const MCLCPPLIB_NAMESPACE::UserProfile& user_profile);
		}
	}
}

#endif // !MINECRAFTLAUNCHERLIB_COMMANDLINE_H_
