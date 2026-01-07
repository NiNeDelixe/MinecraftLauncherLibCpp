#ifndef MINECRAFTLAUNCHERLIB_ARGUMENTS_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENTS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/StringUtils.h"
#include "MinecraftLauncherLib/Utils/Macros.h"

#include "MinecraftLauncherLib/Types/Vector.h"

#include "MinecraftLauncherLib/Core/Argument.h"
#include "MinecraftLauncherLib/Core/Rules.h"
#include "MinecraftLauncherLib/UserOptions.h"
#include "MinecraftLauncherLib/UserProfile.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace arguments
	{
		class MCLLIB_API Arguments
		{
		public:
			static Arguments getArgumentsFromJson(const nlohmann::json& data, const nlohmann::json& versionData, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths);
			static std::u32string replaceArguments(const std::u32string& replace, const nlohmann::json& data, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths);

		public:
			void add(const utils::arguments::Argument& arg)
			{
				arguments.emplace_back(arg);
			}
			types::Vector<std::u32string> toU32StringsVector() const;
			types::Vector<utils::arguments::Argument> getArguments() { return this->arguments; }

		private:
			static void processValue(const nlohmann::json& value, std::u32string& previous, types::Vector<utils::arguments::Argument>& arglist,
				const nlohmann::json& versionData, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths);

		protected:
			types::Vector<utils::arguments::Argument> arguments;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENTS_H_