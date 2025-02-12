#ifndef UTILS_MACROS_H_
#define UTILS_MACROS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>

#include "MinecraftLauncherLib/Utils/PathUtils.h"

#if defined(_WIN32) || defined(_WIN64)
#	define OS std::string("windows")
#elif defined(__APPLE__)
#	define OS std::string("mac")
#elif defined(__unix__) || defined(__unix)
#	define OS std::string("linux")
#else
#	error unsupported platform
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#	define ARCH std::string("x64")
#elif defined(__APPLE__)
#	define ARCH std::string("x86")
#endif

#if defined(_WIN32) || defined(_WIN64)
#	define _UNPACK200 std::string("unpack200.exe")
#else
#	define _UNPACK200 std::string("unpack200")
#endif

#if defined(_WIN32) || defined(_WIN64)
#	define _UNPACK200_ARGS std::string(R"(-r -v -l "")")
#else
#	define _UNPACK200_ARGS std::string("")
#endif

#define _USER_DIR MCLCPPLIB_NAMESPACE::utils::expandUser("~")
#define _JRE_DIR _USER_DIR + "\\" + ".jre"
#define _JDK_DIR _USER_DIR + "\\" + ".jdk"

#define _TAR std::string(".tar")
#define _TAR_GZ std::string(".tar.gz")
#define _ZIP std::string(".zip")
#define _SEVEN_ZIP std::string(".7z")

#define IN_BUF_MAX 409600
#define OUT_BUF_MAX 409600

#define COMPRESSION_LEVEL 7

#define COMPRESSION_EXTREME true

#if defined(_WIN32) || defined(_WIN64)
#	define CLASSPATH_SEPARATOR ';'
#else
#	define CLASSPATH_SEPARATOR ':'
#endif

#endif // !UTILS_MACROS_H_
