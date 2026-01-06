#ifndef MINECRAFTLAUNCHERLIB_USERPROFILE_H_
#define MINECRAFTLAUNCHERLIB_USERPROFILE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>

#include "MinecraftLauncherLib/UserOptions.h"

namespace MCLCPPLIB_NAMESPACE
{
	class MCLLIB_API UserProfile
	{
	public:
		UserProfile(options::RuntimeOptions options)
			: options(options) {}

	public:
		const options::RuntimeOptions& getOptions() const { return options; }
		const std::u32string& getUsername() const { return username; }
		const std::u32string& getUuid() const { return uuid; }
		const std::u32string& getAccessToken() const { return access_token; }
		const std::u32string& getClientId() const { return client_id; }
		const std::u32string& getXuid() const { return xuid; }	

	private:
		std::u32string username;

		std::u32string uuid;
		std::u32string access_token;
		std::u32string client_id;
		std::u32string xuid;

		options::RuntimeOptions options;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_USERPROFILE_H_
