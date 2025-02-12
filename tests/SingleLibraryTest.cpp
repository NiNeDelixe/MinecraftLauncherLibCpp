#define BOOST_TEST_MODULE SingleLibraryTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/Core/SingleLibrary.h"

BOOST_AUTO_TEST_CASE(SingleLibraryPathTest1)
{
    std::filesystem::path libspath = "C:\\test\\mc";
    auto lib = MCLCPPLIB_NAMESPACE::libraries::SingleLibrary::getLibraryFromName(std::string("lox:sd:aa"), libspath);

    const std::filesystem::path final_path = libspath / "libraries" / "lox" / "sd" / "aa" / "sd-aa.jar";
    BOOST_CHECK_EQUAL_COLLECTIONS(final_path.begin(), final_path.end(), lib.getLibraryPath().begin(), lib.getLibraryPath().end());
}

BOOST_AUTO_TEST_CASE(SingleLibraryPathTest2)
{
    std::filesystem::path libspath = "C:\\test\\mc";
    auto lib = MCLCPPLIB_NAMESPACE::libraries::SingleLibrary::getLibraryFromName(std::wstring(L"lox:sd:aa"), libspath);

    const std::filesystem::path final_path = libspath / "libraries" / "lox" / "sd" / "aa" / "sd-aa.jar";
    BOOST_CHECK_EQUAL_COLLECTIONS(final_path.begin(), final_path.end(), lib.getLibraryPath().begin(), lib.getLibraryPath().end());
}

BOOST_AUTO_TEST_CASE(SingleLibraryPathTest3)
{
    std::filesystem::path libspath = "C:\\test\\mc";
    auto lib = MCLCPPLIB_NAMESPACE::libraries::SingleLibrary::getLibraryFromName(std::u32string(U"lox:sd:aa"), libspath);

    const std::filesystem::path final_path = libspath / "libraries" / "lox" / "sd" / "aa" / "sd-aa.jar";
    BOOST_CHECK_EQUAL_COLLECTIONS(final_path.begin(), final_path.end(), lib.getLibraryPath().begin(), lib.getLibraryPath().end());
}

BOOST_AUTO_TEST_CASE(SingleLibraryPathTestWithExtra1)
{
    std::filesystem::path libspath = "C:\\test\\mc";
    auto lib = MCLCPPLIB_NAMESPACE::libraries::SingleLibrary::getLibraryFromName(std::u32string(U"de.oceanlabs.mcp:mcp_config:1.19.2-20220805.130853:mappings@txt"), libspath);

    const std::filesystem::path final_path = libspath / "libraries" / "de" / "oceanlabs" / "mcp"/ "mcp_config" / "1.19.2-20220805.130853" / "mcp_config-1.19.2-20220805.130853-mappings.txt";
    BOOST_CHECK_EQUAL_COLLECTIONS(final_path.begin(), final_path.end(), lib.getLibraryPath().begin(), lib.getLibraryPath().end());
}