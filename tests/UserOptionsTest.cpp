#define BOOST_TEST_MODULE UserOptionsTest
#include <boost/test/unit_test.hpp>

#include "MinecraftLauncherLib/UserOptions.h"

BOOST_AUTO_TEST_CASE(UserOptionsTest1)
{
	nlohmann::json options_json = R"(
	{
		"is_demo": true 
	}
	)"_json;

	//utf doesnt parsing in _json literals
	options_json["minecraft_directory"] = "C:/я\\яя\\ffff/f";

	auto options = MCLCPPLIB_NAMESPACE::options::RuntimeOptions::fromJson(options_json);

	BOOST_CHECK_EQUAL(options.is_demo.get(), options_json["is_demo"].template get<bool>());
	BOOST_CHECK_EQUAL(options.minecraft_directory.get(), options_json["minecraft_directory"].template get<std::filesystem::path>());
}

BOOST_AUTO_TEST_CASE(UserOptionsTest2)
{
	nlohmann::json options_json = R"(
	{
		"is_demo": true 
	}
	)"_json;

	//utf doesnt parsing in _json literals
	options_json["minecraft_directory"] = "C:/я\\яя\\ffff/f";

	auto options = MCLCPPLIB_NAMESPACE::options::RuntimeOptions::fromJson(options_json);

	auto ret_opt_json = options.toJson();

	BOOST_CHECK_EQUAL(ret_opt_json["is_demo"].template get<bool>(), options_json["is_demo"].template get<bool>());
	BOOST_CHECK_EQUAL(ret_opt_json["minecraft_directory"].template get<std::filesystem::path>(), options_json["minecraft_directory"].template get<std::filesystem::path>());
}