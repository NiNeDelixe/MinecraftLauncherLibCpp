#define BOOST_TEST_MODULE VersionProfileTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/VersionProfile.h"

BOOST_AUTO_TEST_CASE(VersionProfileTest1)
{
	MCLCPPLIB_NAMESPACE::VersionProfile client_profile(".");
	client_profile.fromJson(".");
}