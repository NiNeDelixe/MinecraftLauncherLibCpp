#include "MinecraftLauncherLib/Core/Rules.h"

bool MCLCPPLIB_NAMESPACE::rules::Rules::parseRuleList(const nlohmann::json& data, const std::string& rule_string)
{
	/*
	Parse a list of rules
	*/

	if (!data.contains(rule_string))
	{
		return true;
	}

	for (auto& var : data[rule_string])
	{
		if (var.type() != nlohmann::json::value_t::object)
		{
			continue;
		}

		if (!parseSingleRule(var))
		{
			return false;
		}
	}

	return true;
}

bool MCLCPPLIB_NAMESPACE::rules::Rules::parseSingleRule(const nlohmann::json& rule)
{
    /*
    Parse a single rule from the versions.json
    */

    bool returnvalue = rule["action"].template get<std::string>() == "disallow";

    if (rule.contains("os"))
    {
        for (auto& var : rule["os"].items())
        {
            if (var.key() == "name")
            {
                std::string os = var.value().template get<std::string>();
                if ((os == "windows" && OS != "windows") ||
                    (os == "osx" && OS != "darwin") ||
                    (os == "linux" && OS != "linux"))
                {
                    return returnvalue;
                }
            }
            else if (var.key() == "arch" && var.value().template get<std::string>() == "x86" && ARCH != "x86")
            {
                return returnvalue;
            }
            else if (var.key() == "version" && var.value().template get<std::string>() != OS)
            {
                return returnvalue;
            }
        }
    }

    if (rule.contains("features"))
    {
        /*for (auto& var : rule["features"].items())
        {
            if ((var.key() == "has_custom_resolution" && !options.get("customResolution", false)) ||
                (var.key() == "is_demo_user" && !options.get("demo", false)))
            {
                return returnvalue;
            }
        }*/
    }

    return !returnvalue;
}
