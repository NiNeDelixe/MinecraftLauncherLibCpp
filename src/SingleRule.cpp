#include "MinecraftLauncherLib/SingleRule.h"

bool MCLCPPLIB_NAMESPACE::rules::SingleRule::parseSingleRule(SingleRule rule)
{
	/*
	Parse a single rule from the versions.json
	*/
	bool returnvalue = false;
	if (rule.action == "allow")
	{
		returnvalue = false;
	}
	else if (rule.action == "disallow")
	{
		returnvalue = true;
	}

	if (rule.os.has_value())
	{
		if (rule.os.value().name.has_value())
		{
			if (rule.os.value().name.value() == "windows" && OS != "Windows")
			{
				return returnvalue;
			}
			else if (rule.os.value().name.value() == "osx" && OS != "Darwin")
			{
				return returnvalue;
			}
			else if (rule.os.value().name.value() == "linux" && OS != "Linux")
			{
				return returnvalue;
			}
		}
		else if (rule.os.value().arch.has_value())
		{
			if (rule.os.value().arch.value() == "x86" && ARCH != "x86")
			{
				return returnvalue;
			}
		}
		else if (rule.os.value().version.has_value())
		{
			if (rule.os.value().version.value() != OS)
			{
				return returnvalue;
			}
		}
		
	}

	if (rule.features.has_value())
	{
		//FIXME: maybe errors without options
		if (rule.features.value().has_custom_resolution)
		{
			return returnvalue;
		}
		else if (rule.features.value().is_demo_user)
		{
			return returnvalue;
		}
	}
	return !returnvalue;
}

bool MCLCPPLIB_NAMESPACE::rules::SingleRule::parseSingleRule(const nlohmann::json& rule)
{
	bool returnvalue = false;
	if (rule["action"].template get<std::string>() == "allow")
	{
		returnvalue = false;
	}
	else if (rule["action"].template get<std::string>() == "disallow")
	{
		returnvalue = true;
	}
	if (rule.contains("os"))
	{
		for (auto& var : rule["os"].items())
		{
			if (var.key() == "name")
			{
				if (var.value().template get<std::string>() == "windows" && OS != "Windows")
				{
					return returnvalue;
				}
				else if (var.value().template get<std::string>() == "osx" && OS != "Darwin")
				{
					return returnvalue;
				}
				else if (var.value().template get<std::string>() == "linux" && OS != "Linux")
				{
					return returnvalue;
				}
			}
			else if (var.key() == "arch")
			{
				if (var.value().template get<std::string>() == "x86" && ARCH != "x86")
				{
					return returnvalue;
				}
			}
			else if (var.key() == "version")
			{
				if (var.value().template get<std::string>() != OS)
				{
					return returnvalue;
				}
			}
		}
	}
	if (rule.contains("features"))
	{
		for (auto& var : rule["features"].items())
		{
			if (var.key() == "has_custom_resolution" && false /*!options.get("customResolution", false)*/)
			{
				return returnvalue;
			}
			else if (var.key() == "is_demo_user" && false /*!options.get("demo", false)*/)
			{
				return returnvalue;
			}
		}
	}
	return !returnvalue;
}