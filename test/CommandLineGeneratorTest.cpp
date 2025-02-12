#define BOOST_TEST_MODULE CommandLineGeneratorTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/CommandLine.h"

BOOST_AUTO_TEST_CASE(CommandLineGeneratorTest1)
{
    mcllib::UserProfile user;
    mcllib::VersionProfile version;
    auto command = mcllib::commandline::generateCommandLine(user, version);

    BOOST_CHECK_EQUAL(0, 0);
}
