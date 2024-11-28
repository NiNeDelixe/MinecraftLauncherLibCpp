#include "MinecraftLauncherLib/Java.h"

std::filesystem::path MCLCPPLIB_NAMESPACE::java::Java::getExecutablePath(const std::string& jvm_version, const std::filesystem::path& minecraft_directory)
{
    /*
    Returns the path to the executable. Returns None if none is found.
    */
    std::filesystem::path java_path = minecraft_directory / "runtime" / jvm_version / getJvmPlatformString() / jvm_version / "bin" / "java";

    if (std::filesystem::exists(java_path) && !std::filesystem::is_directory(java_path))
    {
        return java_path;
    }

    std::filesystem::path exe_path = java_path.u32string() + U".exe";
    if (std::filesystem::exists(exe_path) && !std::filesystem::is_directory(exe_path))
    {
        return exe_path;
    }

    java_path = std::regex_replace(java_path.wstring(), std::wregex((std::filesystem::path("bin") / "java").wstring()),
        (std::filesystem::path("jre.bundle") / "Contents" / "Home" / "bin" / "java").wstring());

    if (std::filesystem::exists(java_path) && !std::filesystem::is_directory(java_path))
    {
        return java_path;
    }

    return "";
}

std::string MCLCPPLIB_NAMESPACE::java::Java::getJvmPlatformString()
{
	/*
	Get the name that is used to identify the platform
	*/
	if (OS == "windows")
	{
		return (ARCH == "x86") ? "windows-x86" : "windows-x64";
	}
	else if (OS == "linux")
	{
		return (ARCH == "x86") ? "linux-i386" : "linux";
	}
	else if (OS == "mac")
	{
		return "mac-os";
	}
	return "";
}
