#ifndef MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_
#define MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include "MinecraftLauncherLib/Core/Arguments.h"
#include "MinecraftLauncherLib/UserProfile.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace arguments
	{
		class MCLLIB_API UserArguments : public arguments::Arguments
		{
		public:
			void fromUserProfile(const UserProfile& user_info);
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_USERARGUMENTS_H_