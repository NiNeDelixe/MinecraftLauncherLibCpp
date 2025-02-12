#pragma once
#ifndef MINECRAFTLAUNCHERLIB_USEROPTIONS_H_
#define MINECRAFTLAUNCHERLIB_USEROPTIONS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>
#include <ostream>

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
				auto& result = *this;
				if (json.is_object())
				{
					SET_JSON_VALUE(json, result, minecraft_directory, std::filesystem::path);
					SET_JSON_VALUE(json, result, natives_directory, std::filesystem::path);
					SET_JSON_VALUE(json, result, libraries_directory, std::filesystem::path);
					SET_JSON_VALUE(json, result, assets_directory, std::filesystem::path);
					SET_JSON_VALUE(json, result, java_executable_path, std::filesystem::path);

					SET_JSON_VALUE(json, result, is_custom_resolution, bool);
					SET_JSON_VALUE(json, result, custom_resolution_height, int);
					SET_JSON_VALUE(json, result, custom_resolution_width, int);

					SET_JSON_VALUE(json, result, is_demo, bool);

					SET_JSON_VALUE(json, result, server, std::string);
					SET_JSON_VALUE(json, result, port, std::string);

					SET_JSON_VALUE(json, result, launcher_name, std::string);
					SET_JSON_VALUE(json, result, launcher_version, std::string);
				}
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

		class ProcessOptions
		{
		public:
			//std::filesystem::path ;
			uint16_t pid = 0;

			std::ostream* cout_stream = nullptr;
			std::ostream* cerr_stream = nullptr;
		};

	}
}

#endif // !MINECRAFTLAUNCHERLIB_USEROPTIONS_H_
