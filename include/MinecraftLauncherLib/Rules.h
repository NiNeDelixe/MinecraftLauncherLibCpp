#ifndef MINECRAFTLAUNCHERLIB_RULES_H_
#define MINECRAFTLAUNCHERLIB_RULES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/SingleRule.h"
#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace rules
	{
		class Rules
		{
		public:
			Rules() = default;
			~Rules() = default;

		public:
			static Rules fromJson(const nlohmann::json& json);

		public:
			/// <summary>
			/// Parse a list of rules
			/// </summary>
			/// <param name="data"></param>
			/// <param name="rule_string"></param>
			/// <returns></returns>
			static bool parseRuleList(Rules& data, const std::string& rule_string);
			static bool parseRuleList(const nlohmann::json& data, const std::string& rule_string);

		private:
			types::Vector<SingleRule> rules;
			std::string rule_string;
		};
	}
}

#endif //MINECRAFTLAUNCHERLIB_RULES_H_