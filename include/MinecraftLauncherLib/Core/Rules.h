#pragma once
#ifndef MINECRAFTLAUNCHERLIB_RULES_H_
#define MINECRAFTLAUNCHERLIB_RULES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace rules
	{
		class MCLLIB_API Rules
		{
		public:
			Rules() = default;
			~Rules() = delete;

			static bool parseRuleList(const nlohmann::json& data, const std::string& rule_string);
			static bool parseSingleRule(const nlohmann::json& rule);

		private:

		};

	}
}

#endif //MINECRAFTLAUNCHERLIB_RULES_H_