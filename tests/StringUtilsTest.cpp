#define BOOST_TEST_MODULE StringUtilsTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/Utils/StringUtils.h"

BOOST_AUTO_TEST_CASE(StringReplaceTest1)
{
    std::wstring repl = L"asdwordasd";
    std::wstring from = L"dword";
    std::string to = "str";
    MCLCPPLIB_NAMESPACE::utils::string::replace(repl, from, to);
    const std::wstring reference(L"asstrasd");
    BOOST_CHECK_EQUAL_COLLECTIONS(reference.begin(), reference.end(), repl.begin(), repl.end());
}

BOOST_AUTO_TEST_CASE(StringReplaceTest2)
{
    std::u32string repl_32 = U"asdwordasd";
    std::u32string from_32 = U"dword";
    std::u32string to_32 = U"str";
    MCLCPPLIB_NAMESPACE::utils::string::replace(repl_32, from_32, to_32);
    const std::u32string reference_32(U"asstrasd");
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_32.begin(), reference_32.end(), repl_32.begin(), repl_32.end());
}

BOOST_AUTO_TEST_CASE(StringReplaceTest3)
{
    std::wstring repl_w = L"asdwordasd";
    std::wstring from_w = L"dword";
    std::wstring to_w = L"str";
    MCLCPPLIB_NAMESPACE::utils::string::replace(repl_w, from_w, to_w);
    const std::wstring reference_w(L"asstrasd");
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w.begin(), reference_w.end(), repl_w.begin(), repl_w.end());
}

BOOST_AUTO_TEST_CASE(StringReplaceTest4)
{
    std::wstring repl_w = L"asdwordasd";
    std::string from_s = "dword";
    std::string to_s = "str";
    MCLCPPLIB_NAMESPACE::utils::string::replace(repl_w, from_s, to_s);
    const std::wstring reference_w(L"asstrasd");
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w.begin(), reference_w.end(), repl_w.begin(), repl_w.end());
}

BOOST_AUTO_TEST_CASE(StringReplaceTest5)
{
    std::wstring repl_w = L"asdwordasd";
    std::string from_s = "dword";
    std::wstring to_w = L"str";
    MCLCPPLIB_NAMESPACE::utils::string::replace(repl_w, from_s, to_w);
    const std::wstring reference_w(L"asstrasd");
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w.begin(), reference_w.end(), repl_w.begin(), repl_w.end());
}

BOOST_AUTO_TEST_CASE(StringSplitTest1)
{
    std::wstring sep_w = L"ass:ass";

    auto result = MCLCPPLIB_NAMESPACE::utils::string::split(sep_w, ':', 0);
    const std::vector<std::wstring> reference_w = { L"ass", L"ass"};
    
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w[0].begin(), reference_w[0].end(), result[0].begin(), result[0].end());
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w[1].begin(), reference_w[1].end(), result[1].begin(), result[1].end());
}

BOOST_AUTO_TEST_CASE(StringSplitTest2)
{
    std::string sep_w = "ass:ass";

    auto result = MCLCPPLIB_NAMESPACE::utils::string::split(sep_w, ':', 0);
    const std::vector<std::string> reference_w = { "ass", "ass" };

    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w[0].begin(), reference_w[0].end(), result[0].begin(), result[0].end());
    BOOST_CHECK_EQUAL_COLLECTIONS(reference_w[1].begin(), reference_w[1].end(), result[1].begin(), result[1].end());
}