#ifndef BASIC_NATIVES_H_
#define BASIC_NATIVES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"
#include "MinecraftLauncherLib/Utils/StringUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace natives
	{
		class Natives
		{
		public:
			Natives() = default;
			~Natives() = default;

		public:
			static std::string getNativesFromJson(const nlohmann::json& data);

		private:

		};
	}
}

#endif //BASIC_NATIVES_H_