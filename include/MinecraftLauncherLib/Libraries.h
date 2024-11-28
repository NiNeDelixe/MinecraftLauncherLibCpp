#pragma once
#ifndef MINECRAFTLAUNCHERLIB_LIBRARIES_H_
#define MINECRAFTLAUNCHERLIB_LIBRARIES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>
#include <optional>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Types/Vector.h"
#include "MinecraftLauncherLib/Utils/Macros.h"
#include "MinecraftLauncherLib/Utils/StringUtils.h"

#include "MinecraftLauncherLib/SingleLibrary.h"
#include "MinecraftLauncherLib/Rules.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace libraries
	{
		class Libraries
		{
		public:
			Libraries(const std::filesystem::path& minecraft_dir);
			~Libraries() = default;

			static Libraries fromJson(const nlohmann::json& json, const std::filesystem::path& minecraft_dir, const std::filesystem::path& libraries_root);
			static std::string getNatives(const nlohmann::json& json);
			static std::u32string pathsToString(types::Vector<std::filesystem::path> pahts);

			types::Vector<std::filesystem::path> toPaths();

		private:
			types::Vector<SingleLibrary> libraries;
			std::filesystem::path minecraft_directory;
		};

	}
}

#endif //MINECRAFTLAUNCHERLIB_LIBRARIES_H_