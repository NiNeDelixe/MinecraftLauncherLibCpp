#ifndef MINECRAFTLAUNCHERLIB_MCCLIBAPI_H_
#define MINECRAFTLAUNCHERLIB_MCCLIBAPI_H_

#if defined(MCLLIB_DLL_BUILD)
#	if defined(WIN32) || defined(_WIN32)
#		define MCLLIB_API __declspec(dllexport)
#	else
#		define MCLLIB_API
#	endif
#else
#	if (defined(WIN32) || defined(_WIN32)) && defined(MCLLIB_DLL)
#		define MCLLIB_API __declspec(dllimport)
#	else
#		define MCLLIB_API
#	endif
#endif

#define MCLCPPLIB_NAMESPACE mcllib

#endif /*MINECRAFTLAUNCHERLIB_MCCLIBAPI_H_*/