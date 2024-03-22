#ifndef UTILS_MEMORYUTILS_H_
#define UTILS_MEMORYUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>
#include <string>
#include <locale>
#include <codecvt>
#include <string>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace memory
		{
			
		}

		namespace convert
		{
			std::wstring convertStringToWstring(const std::string& conv);
		}
	}
}

#endif // !UTILS_MEMORYUTILS_H_
