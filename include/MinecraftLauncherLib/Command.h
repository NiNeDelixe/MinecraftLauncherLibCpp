#pragma once
#ifndef MINECRAFTLAUNCHERLIB_COMMAND_H_
#define MINECRAFTLAUNCHERLIB_COMMAND_H_

#include "MCLLib-api.h"

#include <memory>
#include <variant>

#include "MinecraftLauncherLib/Arguments.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace command
	{
		class Command
		{
		public:
			void pushBack(std::shared_ptr<arguments::Arguments> argument)
			{
				arguments.push_back({ argument });
			}

		public:
			std::u32string toU32String();

		private:
			types::Vector<std::variant<std::shared_ptr<arguments::Arguments>>> arguments;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_COMMAND_H_