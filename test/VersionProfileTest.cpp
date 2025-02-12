#define BOOST_TEST_MODULE VersionProfileTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/VersionProfile.h"

BOOST_AUTO_TEST_CASE(VersionProfileTest1)
{
	auto exec_path = std::filesystem::current_path();
	std::filesystem::path mcpath = exec_path / ".." / ".." / ".." / ".." / "test" / "mc";
	MCLCPPLIB_NAMESPACE::VersionProfile client_profile;
	client_profile.fromJson(mcpath / "versions\\1.18.2\\1.18.2.json");
}