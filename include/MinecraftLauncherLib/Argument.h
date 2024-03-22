#ifndef MINECRAFTLAUNCHERLIB_ARGUMENT_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENT_H_

#include "MCLLib-api.h"

#include <variant>
#include <filesystem>
#include <string>
#include <vector>

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace arguments
		{
			struct Argument
			{
				std::u32string key;
				std::variant<std::u32string, std::filesystem::path, types::Vector<std::filesystem::path>> value;
			};
		}
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENT_H_
