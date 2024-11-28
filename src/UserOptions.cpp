#include "MinecraftLauncherLib/UserOptions.h"

MCLCPPLIB_NAMESPACE::options::RuntimeOptions MCLCPPLIB_NAMESPACE::options::RuntimeOptions::fromJson(const nlohmann::json& json)
{
    MCLCPPLIB_NAMESPACE::options::RuntimeOptions result;

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

	return result;
}

nlohmann::json MCLCPPLIB_NAMESPACE::options::RuntimeOptions::toJson() const
{
    nlohmann::json result;

    ADD_TO_JSON(result, minecraft_directory);
    ADD_TO_JSON(result, natives_directory);
    ADD_TO_JSON(result, libraries_directory);
    ADD_TO_JSON(result, assets_directory);
    ADD_TO_JSON(result, java_executable_path);

    ADD_TO_JSON(result, is_custom_resolution);
    ADD_TO_JSON(result, custom_resolution_height);
    ADD_TO_JSON(result, custom_resolution_width);

    ADD_TO_JSON(result, is_demo);

    ADD_TO_JSON(result, server);
    ADD_TO_JSON(result, port);

    ADD_TO_JSON(result, launcher_name);
    ADD_TO_JSON(result, launcher_version);

    return result;
}
