#define BOOST_TEST_MODULE SingleLibraryTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/SingleLibrary.h"

BOOST_AUTO_TEST_CASE(SingleLibraryPathTest1)
{
    std::filesystem::path libspath = "C:\\test\\mc";
    auto lib = MCLCPPLIB_NAMESPACE::SingleLibrary::getLibraryFromName(std::string("lox:sd:aa"), libspath);

    const std::filesystem::path final_path = libspath / "libraries" / "lox" / "sd" / "aa" / "sd-aa.jar";
    BOOST_CHECK_EQUAL_COLLECTIONS(final_path.begin(), final_path.end(), lib.getLibraryPath().begin(), lib.getLibraryPath().end());
}
