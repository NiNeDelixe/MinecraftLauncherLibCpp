#ifndef MINECRAFTLAUNCHERLIB_USERPROFILE_H_
#define MINECRAFTLAUNCHERLIB_USERPROFILE_H_

#include "MCLLib-api.h"

#include <string>

namespace MCLCPPLIB_NAMESPACE
{
	class UserProfile
	{
	private:
		std::u32string username;

		std::u32string uuid;
		std::u32string access_token;
		std::u32string client_id;
		std::u32string xuid;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_USERPROFILE_H_
