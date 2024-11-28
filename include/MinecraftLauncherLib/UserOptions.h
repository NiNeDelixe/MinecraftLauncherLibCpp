#pragma once
#ifndef MINECRAFTLAUNCHERLIB_USEROPTIONS_H_
#define MINECRAFTLAUNCHERLIB_USEROPTIONS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Setting.h"

#define SET_JSON_VALUE(json, result, field, type) \
    if (json.contains(#field)) { \
        result.field.set(json[#field].template get<type>()); \
    }

#define ADD_TO_JSON(json, field) json[#field] = this->field.get();

namespace MCLCPPLIB_NAMESPACE
{
	namespace options
	{
		struct RuntimeOptions
		{
		private:
			RuntimeOptions() = default;

		public:
			RuntimeOptions(const nlohmann::json& json)
			{
				fromJson(json);
			}
			~RuntimeOptions() = default;

			static RuntimeOptions fromJson(const nlohmann::json& json);

			nlohmann::json toJson() const;
			static nlohmann::json toJson(const RuntimeOptions& options) { return options.toJson(); }

			//FIXME: linux
			utils::settings::PathSetting minecraft_directory = { std::filesystem::temp_directory_path()
				.parent_path().parent_path().parent_path() / "Roaming" / ".minecraft" };
			utils::settings::PathSetting libraries_directory = { minecraft_directory.get() / "libraries" };
			utils::settings::PathSetting natives_directory = { minecraft_directory.get() / "natives" };
			utils::settings::PathSetting assets_directory = { minecraft_directory.get() / "assets" };
			utils::settings::PathSetting java_executable_path = { "java" };

			utils::settings::FlagSetting is_custom_resolution = { false };
			utils::settings::NumberSetting custom_resolution_height = { 480, 0 };
			utils::settings::NumberSetting custom_resolution_width = { 854, 0 };

			utils::settings::FlagSetting is_demo = { false };

			utils::settings::StringSetting server = { "null" };
			utils::settings::StringSetting port = { "null" };

			utils::settings::StringSetting launcher_name = { "null" };
			utils::settings::StringSetting launcher_version = { "null" };
		};

	}
}

#endif // !MINECRAFTLAUNCHERLIB_USEROPTIONS_H_
