#ifndef BASIC_MINECRAFTOBJECT_H_
#define BASIC_MINECRAFTOBJECT_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

namespace MCLCPPLIB_NAMESPACE
{
	namespace basic
	{
		class MinecraftObject
		{
		public:
			MinecraftObject() = default;
			~MinecraftObject() = default;

		public:
			virtual void fromJson(const std::filesystem::path& json_path) {};
			virtual void fromJson(const nlohmann::json& json) {};
			virtual nlohmann::json inheritJson(const nlohmann::json& original_data, const std::filesystem::path& minecraft_directory);

		private:

		};
	}
}

#endif // !BASIC_MINECRAFTOBJECT_H_
