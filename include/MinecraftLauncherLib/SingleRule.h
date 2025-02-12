#ifndef MINECRAFTLAUNCHERLIB_SINGLERULE_H_
#define MINECRAFTLAUNCHERLIB_SINGLERULE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <optional>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace rules
	{
		class SingleRule
		{
		public:
			SingleRule() = default;
			~SingleRule() = default;

		public:
			static bool parseSingleRule(SingleRule rule);
			static bool parseSingleRule(const nlohmann::json& rule);

		protected:
			struct Features
			{
				bool has_custom_resolution = false;
				bool is_demo_user = false;
			};

		protected:
			struct _OS
			{
				std::optional<std::string> name;
				std::optional<std::string> arch;
				std::optional<std::string> version;
			};

		private:
			std::string action;
			std::optional<_OS> os;
			std::optional<Features> features;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_SINGLERULE_H_
