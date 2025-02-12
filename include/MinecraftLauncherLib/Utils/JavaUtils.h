#pragma once
#ifndef UTILS_JAVAUTILS_H_
#define UTILS_JAVAUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <string>
#include <vector>

#ifdef JNI_ENABLED
#include <jni.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#endif

#include "MinecraftLauncherLib/Core/Command.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace java
		{
#ifdef JNI_ENABLED
			static void startJava(const MCLCPPLIB_NAMESPACE::command::Command& command)
			{
				std::string class_paths_option = "-Djava.class.path=" + command.getMCLibraries().toString();

				std::vector<JavaVMOption> options;
				//options.push_back({ const_cast<char*>(class_paths_option.c_str()) });

				for (const auto& arg : command.getJavaArguments().toU32StringsVector())
				{
					std::filesystem::path arg_p = std::filesystem::path(arg);
					std::string arg_s = arg_p.string();
					char* arg_nc = const_cast<char*>(arg_s.c_str());
					options.push_back({ arg_nc });
				}

				/*std::string verbose = "-verbose:jni";
				options.push_back({ const_cast<char*>(verbose.c_str()) });*/

				JavaVMInitArgs vm_args{};
				vm_args.version = JNI_VERSION_1_8;
				vm_args.nOptions = static_cast<jint>(options.size());
				vm_args.options = options.data();
				vm_args.ignoreUnrecognized = false;

				JavaVM* jvm = nullptr;
				JNIEnv* env = nullptr;

				JNI_GetDefaultJavaVMInitArgs(&vm_args);

				jint res = JNI_CreateJavaVM(&jvm, (void**)(&env), &vm_args);
				if (res != JNI_OK)
				{
					throw std::runtime_error("Failed to create Java VM. Error code: " + res);
				}

				jclass javaClass = env->FindClass(command.getJavaClassName().string().c_str());
				if (!javaClass)
				{
					throw std::runtime_error("Failed to find class: " + command.getJavaClassName().string());
					jvm->DestroyJavaVM();
				}

				jmethodID mainMethod = env->GetStaticMethodID(javaClass, "main", "([Ljava/lang/String;)V");
				if (!mainMethod)
				{
					throw std::runtime_error("Failed to find method: main");
					jvm->DestroyJavaVM();
				}

				jobjectArray args = env->NewObjectArray(
					static_cast<jsize>(command.getUserArguments().toU32StringsVector().size()),
					env->FindClass("java/lang/String"),
					nullptr
				);

				size_t iter = 0;
				for (const auto& uarg : command.getUserArguments().toU32StringsVector())
				{
					std::filesystem::path arg_p = std::filesystem::path(uarg);
					std::string arg_s = arg_p.string();
					jstring arg = env->NewStringUTF(arg_s.c_str());
					env->SetObjectArrayElement(args, static_cast<jsize>(iter), arg);
					++iter;
				}

				env->CallStaticVoidMethod(javaClass, mainMethod, args);

				if (env->ExceptionOccurred())
				{
					env->ExceptionDescribe();
					env->ExceptionClear();
					jvm->DestroyJavaVM();
				}

				jvm->DestroyJavaVM();
			}
#else
			static void startJava(const MCLCPPLIB_NAMESPACE::command::Command& command, MCLCPPLIB_NAMESPACE::options::ProcessOptions& options)
			{
#if defined(_WIN32) || defined(_WIN64)
				auto& command_vector = command.toU32StringVector();

				std::wstring programm = std::filesystem::path(*command_vector.begin()).wstring();

				command_vector.erase(command_vector.begin(), ++command_vector.begin());

				std::wstring commandLine = L"\"" + programm + L"\"";
				bool libraries = false;
				for (const auto& arg : command_vector)
				{
					if (arg == U"-cp")
					{
						libraries = true;
						commandLine += L" \"" + std::filesystem::path(arg).wstring() + L"\" ";
						continue;
					}
					if (arg == std::filesystem::path(command.getJavaClassName()).u32string())
						libraries = false;

					if (libraries)
					{
						commandLine += L"\"" + std::filesystem::path(arg).wstring() + L"\"" + std::wstring(CLASSPATH_SEPARATOR);
					}
					else
					{
						commandLine += L" \"" + std::filesystem::path(arg).wstring() + L"\"";
					}
				}

				//FIXME: replace with normal converter
				(*options.cout_stream) << "[Command line]: " << std::filesystem::path(commandLine).string() << "\n";

				// Create pipes
				HANDLE hStdOutRead, hStdOutWrite;
				SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
				if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) 
				{
					throw std::runtime_error("Failed to create pipe");
				}
				SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);

				// Setup STARTUPINFO structure
				STARTUPINFOW si = { sizeof(STARTUPINFO) };
				PROCESS_INFORMATION pi = {};
				si.dwFlags |= STARTF_USESTDHANDLES;
				si.hStdOutput = hStdOutWrite;
				si.hStdError = hStdOutWrite;

				// Start the process
				if (!CreateProcessW(NULL, const_cast<wchar_t*>(commandLine.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) 
				{
					CloseHandle(hStdOutWrite);
					CloseHandle(hStdOutRead);
					throw std::runtime_error("Failed to create process");
				}

				// Close the write end of the pipe in the parent process
				CloseHandle(hStdOutWrite);

				// Read the output from the child process
				char buffer[4096];
				DWORD bytesRead;
				while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) 
				{
					buffer[bytesRead] = '\0';
					(*options.cout_stream) << buffer;
				}

				// Clean up
				CloseHandle(hStdOutRead);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
#else
				auto& command_vector = command.toU32StringVector();

				command_vector.erase(command_vector.begin(), command_vector.begin());

#endif
			}
#endif
		}
	}
}

#endif // UTILS_JAVAUTILS_H_