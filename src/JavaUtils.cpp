#include "MinecraftLauncherLib/Utils/JavaUtils.h"

std::filesystem::path MCLCPPLIB_NAMESPACE::utils::java::getExecutablePath(const std::string& jvm_version, const std::filesystem::path& minecraft_directory)
{
	std::filesystem::path java_path = minecraft_directory / "runtime" / jvm_version / getJvmPlatformString() / jvm_version / "bin" / "java";
	if (std::filesystem::exists(java_path))
	{
		if (!std::filesystem::is_directory(java_path))
		{
			return java_path;
		}
		else if (!std::filesystem::is_directory(java_path.wstring() + L".exe"))
		{
			return java_path.wstring() + L".exe";
		}
	}

	std::filesystem::path java_w_path = java_path;
	java_w_path = std::regex_replace(java_w_path.wstring(), std::wregex((std::filesystem::path("bin") / "java").wstring()), (std::filesystem::path("jre.bundle") / "Contents" / "Home" / "bin" / "java").wstring());
	if (std::filesystem::exists(java_w_path))
	{
		if (!std::filesystem::is_directory(java_w_path))
		{
			return java_w_path;
		}
		else
		{
			return "";
		}
	}
	return "";
}

std::string MCLCPPLIB_NAMESPACE::utils::java::getJvmPlatformString()
{
	if (OS == "windows")
	{
		if (ARCH == "x86")
		{
			return "windows-x86";
		}
		else
		{
			return "windows-x64";
		}
	}
	else if (OS == "linux")
	{
		if (ARCH == "x86")
		{
			return "linux-i386";
		}
		else
		{
			return "linux";
		}
	}
	else if (OS == "mac")
	{
		return "mac-os";
	}
	return "";
}
