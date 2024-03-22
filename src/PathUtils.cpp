#include "MinecraftLauncherLib/Utils/PathUtils.h"

std::filesystem::path MCLCPPLIB_NAMESPACE::utils::expandUser(const std::filesystem::path& path)
{
	if (!path.empty() && path == "~")
	{
		char* home = nullptr;
		size_t home_path_size = 0;
		_dupenv_s(&home, &home_path_size, "HOME");

		if (home && (_dupenv_s(&home, &home_path_size, "USERPROFILE")) && home != NULL)
		{
			MCLCPPLIB_NAMESPACE::utils::string::replace(path.wstring(), path.wstring(), std::string(home));
			//std::replace(path.begin(), path.end(), path.string(), std::string(home));
		}
		else
		{
			char* home_drive = nullptr;
			size_t home_drive_size = 0;

			char* home_path = nullptr;
			size_t home_path_size = 0;

			_dupenv_s(&home_drive, &home_drive_size, "HOMEDRIVE");
			_dupenv_s(&home_path, &home_path_size, "HOMEPATH");

			MCLCPPLIB_NAMESPACE::utils::string::replace(path.wstring(), path.wstring(), (home_drive == nullptr ? "" : std::string(home_drive)) + (home_path == nullptr ? "" : home_path));
		}
	}
    return path;
}
