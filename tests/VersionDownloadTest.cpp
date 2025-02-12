#define BOOST_TEST_MODULE VersionDownloadTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/Download/VanillaDownloader.h"
#include "MinecraftLauncherLib/Download/ForgeDownloader.h"
#include "MinecraftLauncherLib/Download/FabricDownloader.h"
//#include "MinecraftLauncherLib/Utils/JavaUtils.h"

//BOOST_AUTO_TEST_CASE(VersionDownloadTest1)
//{
//	nlohmann::json options = R"(
//{
//	"minecraft_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
//	"libraries_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
//	"assets_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka"
//}
//)"_json;
//
//	MCLCPPLIB_NAMESPACE::download::VanillaDownloader downloader("1.19.2", std::filesystem::temp_directory_path(), options, nullptr);
//	downloader.installLoader();
//	downloader.update();
//
//	//BOOST_CHECK_EQUAL_COLLECTIONS(f.begin(), f.end(), s.begin(), s.end());
//	//BOOST_CHECK_EQUAL(left, right);
//}

BOOST_AUTO_TEST_CASE(VersionDownloadTestForge)
{
	nlohmann::json options = R"(
{
	"java_executable_path": "D:\\ProgramFiles\\jdk1.8\\bin\\java.exe",
	"minecraft_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
	"libraries_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
	"assets_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka"
}
)"_json;

	MCLCPPLIB_NAMESPACE::download::loaders::ForgeDownloader downloader("1.19.2", "43.4.6", std::filesystem::temp_directory_path(), options, nullptr);
	downloader.installLoader();
	downloader.update();

	//BOOST_CHECK_EQUAL_COLLECTIONS(f.begin(), f.end(), s.begin(), s.end());
	//BOOST_CHECK_EQUAL(left, right);
}

//BOOST_AUTO_TEST_CASE(VersionDownloadTestFabric)
//{
//	nlohmann::json options = R"(
//{
//	"java_executable_path": "D:\\ProgramFiles\\jdk1.8\\bin\\java.exe",
//	"minecraft_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
//	"libraries_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka",
//	"assets_directory": "C:\\Users\\NanLeb\\AppData\\Roaming\\.SomSomSom\\suka"
//}
//)"_json;
//
//	MCLCPPLIB_NAMESPACE::download::loaders::FabricDownloader downloader("1.19.2", "0.16.10", std::filesystem::temp_directory_path(), options, nullptr);
//	downloader.installLoader();
//
//	//BOOST_CHECK_EQUAL_COLLECTIONS(f.begin(), f.end(), s.begin(), s.end());
//	//BOOST_CHECK_EQUAL(left, right);
//}