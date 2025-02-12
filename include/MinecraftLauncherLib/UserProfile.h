#ifndef MINECRAFTLAUNCHERLIB_USERPROFILE_H_
#define MINECRAFTLAUNCHERLIB_USERPROFILE_H_

#include "MCLLib-api.h"

#include <string>

namespace MCLCPPLIB_NAMESPACE
{
	class UserProfile
	{
	public:
		const std::u32string& getUsername() const { return this->username; }
		const std::u32string& getAccessToken() const { return this->access_token; }
		const std::u32string& getClientId() const { return this->client_id; }
		const std::u32string& getXUID() const { return this->xuid; }
		const std::u32string& getUUID() const { return this->uuid; }

	private:
		std::u32string username;

		std::u32string uuid;
		std::u32string access_token;
		std::u32string client_id;
		std::u32string xuid;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_USERPROFILE_H_
