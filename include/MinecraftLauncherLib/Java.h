#pragma once
#ifndef MINECRAFTLAUNCHERLIB_JAVA_H_
#define MINECRAFTLAUNCHERLIB_JAVA_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <regex>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace java
	{
		class Java
		{
		public:
			Java() = default;
			~Java() = default;

			static std::filesystem::path getExecutablePath(const std::string& jvm_version, const std::filesystem::path& minecraft_directory);

		private:
			static std::string getJvmPlatformString();

		private:

		};


	}
}

#endif //MINECRAFTLAUNCHERLIB_JAVA_H_