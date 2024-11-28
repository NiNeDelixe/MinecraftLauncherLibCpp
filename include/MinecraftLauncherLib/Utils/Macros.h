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
//"unpack200.exe"
#	define _UNPACK200 {0x75, 0x6E, 0x70, 0x61, 0x63, 0x6B, 0x32, 0x30, 0x30, 0x2E, 0x65, 0x78, 0x65}
#else
//"unpack200"
#	define _UNPACK200 {0x75, 0x6E, 0x70, 0x61, 0x63, 0x6B, 0x32, 0x30, 0x30}
#endif

#if defined(_WIN32) || defined(_WIN64)
//"-r -v -l \"\""
#	define _UNPACK200_ARGS {0x2D, 0x72, 0x20, 0x2D, 0x76, 0x20, 0x2D, 0x6C, 0x20, 0x22, 0x22}
#else
//empty
#	define _UNPACK200_ARGS {}
#endif

#define _USER_DIR MCLCPPLIB_NAMESPACE::utils::expandUser("~")
#define _JRE_DIR _USER_DIR + "\\" + ".jre"
#define _JDK_DIR _USER_DIR + "\\" + ".jdk"

//".tar"
#define _TAR {0x2E, 0x74, 0x61, 0x72}
//".tar.gz"
#define _TAR_GZ {0x2E, 0x74, 0x61, 0x72, 0x2E, 0x67, 0x7A}
//".zip"
#define _ZIP {0x2E, 0x7A, 0x69, 0x70}
//".7z"
#define _SEVEN_ZIP {0x2E, 0x37, 0x7A}

#define IN_BUF_MAX 409600
#define OUT_BUF_MAX 409600

#define COMPRESSION_LEVEL 7

#define COMPRESSION_EXTREME true

#if defined(_WIN32) || defined(_WIN64)
//str ";"
#	define CLASSPATH_SEPARATOR {0x3B}
#else
//str ":"
#	define CLASSPATH_SEPARATOR {0x3A}
#endif

#endif // !UTILS_MACROS_H_
