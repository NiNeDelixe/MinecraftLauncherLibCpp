#ifndef MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_
#define MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_

#include "MCLLib-api.h"

#include "MinecraftLauncherLib/Arguments.h"
#include "MinecraftLauncherLib/UserProfile.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace arguments
	{
		class UserArguments : public arguments::Arguments
		{
		public:
			void fromUserProfile(UserProfile user_info);
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_