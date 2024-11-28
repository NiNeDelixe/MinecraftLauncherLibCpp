#include "MinecraftLauncherLib/Libraries.h"

MCLCPPLIB_NAMESPACE::libraries::Libraries::Libraries(const std::filesystem::path& minecraft_dir)
	: minecraft_directory(minecraft_dir)
{
}

MCLCPPLIB_NAMESPACE::libraries::Libraries MCLCPPLIB_NAMESPACE::libraries::Libraries::fromJson(const nlohmann::json& json, const std::filesystem::path& minecraft_dir, const std::filesystem::path& libraries_root)
{
	/*
	Returns the argument with all libs that come after -cp
	*/
	nlohmann::json data = json;
	std::filesystem::path path = libraries_root;
	Libraries libs(minecraft_dir);
	std::string native = "";

	for (auto& elem : data["libraries"])
	{
		if (elem.type() != nlohmann::json::value_t::object)
		{
			continue;
		}
		if (elem.contains("rules") && !(rules::Rules::parseRuleList(elem, "rules")))
		{
			continue;
		}

		libs.libraries.push_back(SingleLibrary::getLibraryFromName(elem["name"].template get<std::string>(), path));
		native = getNatives(elem);

		if (native != "")
		{
			if (elem.contains("downloads"))
			{
				libs.libraries.push_back(
					path / "libraries" / elem["downloads"]["classifiers"][native]["path"].template get<std::string>()
				);
			}
			else
			{
				libs.libraries.push_back(
					SingleLibrary::getLibraryFromName((elem["name"].template get<std::string>() + ("-" + native)), path)
				);
			}
		}
	}

	if (data.contains("jar"))
	{
		libs.libraries.push_back(
			minecraft_dir / "versions" / data["jar"].template get<std::string>() / (data["jar"].template get<std::string>() + ".jar")
		);
	}
	else
	{
		libs.libraries.push_back(
			minecraft_dir / "versions" / data["id"].template get<std::string>() / (data["id"].template get<std::string>() + ".jar")
		);
	}

	return libs;
}

std::string MCLCPPLIB_NAMESPACE::libraries::Libraries::getNatives(const nlohmann::json& json)
{
	nlohmann::json data = json;
	std::string arch_type = ARCH == "x86" ? "32" : "64";

	if (!(data.type() == nlohmann::json::value_t::object && data.contains("natives")))
	{
		return "";
	}
	
	if (OS == "windows")
	{
		if (!(data.type() != nlohmann::json::value_t::object && data["natives"].contains("windows")))
		{
			return "";
		}
		
		std::string replace_string = data["natives"]["windows"].template get<std::string>();
		utils::string::replace(replace_string, std::string("${arch}"), arch_type);
		return replace_string;
	}
	else if (OS == "mac")
	{
		if (data.type() != nlohmann::json::value_t::object && data["natives"].contains("osx"))
		{
			return "";
		}

		std::string replace_string = data["natives"]["osx"].template get<std::string>();
		utils::string::replace(replace_string, std::string("${arch}"), arch_type);
		return replace_string;
	}
	else
	{
		if (data.type() != nlohmann::json::value_t::object && data["natives"].contains("linux"))
		{
			return "";
		}

		std::string replace_string = data["natives"]["linux"].template get<std::string>();
		utils::string::replace(replace_string, std::string("${arch}"), arch_type);
		return replace_string;
	}

	return "";
}

std::u32string MCLCPPLIB_NAMESPACE::libraries::Libraries::pathsToString(types::Vector<std::filesystem::path> pahts)
{
	std::u32string result;
	for (size_t i = 0; i < pahts.size(); i++)
	{
		result += pahts[i].u32string() + std::u32string(CLASSPATH_SEPARATOR);
	}
	return result;
}

MCLCPPLIB_NAMESPACE::types::Vector<std::filesystem::path> MCLCPPLIB_NAMESPACE::libraries::Libraries::toPaths()
{
	types::Vector<std::filesystem::path> paths;
	for (size_t i = 0; i < this->libraries.size(); i++)
	{
		paths.push_back(this->libraries[i].getLibraryPath());
	}
	return paths;
}
