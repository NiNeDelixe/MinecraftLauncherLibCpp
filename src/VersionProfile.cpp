#include "MinecraftLauncherLib/VersionProfile.h"

MCLCPPLIB_NAMESPACE::VersionProfile::VersionProfile(const std::filesystem::path& instance_path)
	: instance_path(instance_path)
{

}

void MCLCPPLIB_NAMESPACE::VersionProfile::fromJson(const std::filesystem::path& json_path)
{
	std::ifstream ifstr(json_path);
	nlohmann::json version_json = nlohmann::json::parse(ifstr);
	ifstr.close();

}