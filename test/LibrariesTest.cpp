#define BOOST_TEST_MODULE LibrariesTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/Libraries.h"

BOOST_AUTO_TEST_CASE(LibrariesTest1)
{
    auto a = std::filesystem::current_path();
    std::filesystem::path mcpath = a / ".." / ".." / ".." / ".." / "test" / "mc";

    std::ifstream ifstr(mcpath / "versions" / "1.18.2" / "1.18.2.json");
    nlohmann::json js = nlohmann::json::parse(ifstr);
    ifstr.close();
    
    auto libs = MCLCPPLIB_NAMESPACE::Libraries::fromJson(js, mcpath);

    BOOST_CHECK_EQUAL(0, 0);
}
