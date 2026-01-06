#ifndef MINECRAFTLAUNCHERLIB_ARGUMENT_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENT_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <variant>
#include <filesystem>
#include <string>
#include <vector>
#include <optional>

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace arguments
		{
			struct MCLLIB_API Argument
			{
				Argument(const std::u32string& key, std::optional<std::u32string> value)
					: key(key), value(value) {}

				Argument(const std::pair<std::u32string, std::optional<std::u32string>> arg)
					: Argument(arg.first, arg.second) {}

				std::u32string key;
				std::optional<std::u32string> value;
			};
		}
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENT_H_
