#define BOOST_TEST_MODULE CommandLineTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/CommandLine.h"

BOOST_AUTO_TEST_CASE(CommandLineTest1)
{
	nlohmann::json options = R"(
{
	"minecraft_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\random-mining-go",
	"libraries_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom",
	"assets_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom"
}
)"_json;

	auto opt = MCLCPPLIB_NAMESPACE::options::RuntimeOptions::fromJson(options);
	MCLCPPLIB_NAMESPACE::VersionProfile version_profile("1.19.2", opt);
	MCLCPPLIB_NAMESPACE::UserProfile user_profile(opt);
	auto string = MCLCPPLIB_NAMESPACE::command::string::generateCommand(version_profile, user_profile);

	BOOST_TEST_MESSAGE(std::filesystem::path(string).string());
}