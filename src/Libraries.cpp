#include "MinecraftLauncherLib/Libraries.h"

MCLCPPLIB_NAMESPACE::Libraries MCLCPPLIB_NAMESPACE::Libraries::fromJson(const nlohmann::json& data, const std::filesystem::path& path)
{
	Libraries libs;

	std::string native = "";
	std::filesystem::path mc_path = path/*.lexically_normal()*/;

	for (auto& elem : data["libraries"])
	{
		if (elem.type() != nlohmann::json::value_t::object)
		{
			continue;
		}
		if (elem.contains("rules") && !(rules::Rules::parseRuleList(rules::Rules::fromJson(elem), "rules")))
		{
			continue;
		}

		libs.libraries_paths.push_back(SingleLibrary::getLibraryFromName(elem["name"].template get<std::string>(), mc_path));
		native = natives::Natives::getNativesFromJson(elem);

		if (native != "")
		{
			if (elem.contains("downloads"))
			{
				libs.libraries_paths.push_back(SingleLibrary(mc_path / "libraries" / elem["downloads"]["classifiers"][native]["path"].template get<std::filesystem::path>().lexically_normal()));
			}
			else
			{
				libs.libraries_paths.push_back(SingleLibrary::getLibraryFromName((elem["name"].template get<std::string>() + ("-" + native)), mc_path));
			}
		}
	}

	if (data.contains("jar"))
	{
		libs.libraries_paths.push_back(SingleLibrary(mc_path / "versions" / data["jar"].template get<std::string>() / (data["jar"].template get<std::string>() + ".jar")));
	}
	else
	{
		libs.libraries_paths.push_back(SingleLibrary(mc_path / "versions" / data["id"].template get<std::string>() / (data["id"].template get<std::string>() + ".jar")));
	}

	return libs;
}