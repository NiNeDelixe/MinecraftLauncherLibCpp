#include "MinecraftLauncherLib/Rules.h"

MCLCPPLIB_NAMESPACE::rules::Rules MCLCPPLIB_NAMESPACE::rules::Rules::fromJson(const nlohmann::json& json)
{
	Rules rules;
	if (json.contains("rule_string"))
	{
		rules.rule_string = json["rule_string"];
	}
	return rules;
}

bool MCLCPPLIB_NAMESPACE::rules::Rules::parseRuleList(Rules& data, const std::string& rule_string)
{
	if (data.rule_string != rule_string)
	{
		return true;
	}
	for (size_t i = 0; i < data.rules.size(); ++i)
	{
		if (!SingleRule::parseSingleRule(data.rules[i]))
		{
			return false;
		}
	}
	return true;
}

bool MCLCPPLIB_NAMESPACE::rules::Rules::parseRuleList(const nlohmann::json& data, const std::string& rule_string)
{
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
		if (!SingleRule::parseSingleRule(var))
		{
			return false;
		}
	}
	return true;
}