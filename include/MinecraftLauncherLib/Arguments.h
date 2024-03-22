#ifndef MINECRAFTLAUNCHERLIB_ARGUMENTS_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENTS_H_

#include "MCLLib-api.h"

#include "MinecraftLauncherLib/Argument.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace arguments
	{
		class Arguments
		{
		protected:
			types::Vector<utils::arguments::Argument> arguments;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENTS_H_