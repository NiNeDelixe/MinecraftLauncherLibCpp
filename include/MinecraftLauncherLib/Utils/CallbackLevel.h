#pragma once
#ifndef UTILS_CALLBACKLEVEL_H_
#define UTILS_CALLBACKLEVEL_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace callback
		{
			enum CallbackLevel
			{		
				MC_MESSEGE,
				MC_WARRNING,
				MC_ERROR,
				MC_DEBUG
			};
		}
	}
}

#endif // UTILS_CALLBACKLEVEL_H_