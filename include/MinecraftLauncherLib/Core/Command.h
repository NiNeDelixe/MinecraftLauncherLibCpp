#pragma once
#ifndef MINECRAFTLAUNCHERLIB_COMMAND_H_
#define MINECRAFTLAUNCHERLIB_COMMAND_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <memory>
#include <variant>

#include "MinecraftLauncherLib/Core/Arguments.h"
#include "MinecraftLauncherLib/Core/JavaArguments.h"
#include "MinecraftLauncherLib/Core/UserArguments.h"
#include "MinecraftLauncherLib/Core/Java.h"
#include "MinecraftLauncherLib/Core/Libraries.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace command
	{
		class MCLLIB_API Command
		{
		public:
			Command(const std::filesystem::path& minecraft_directory)
				: mc_libraries(minecraft_directory)
			{}
			Command()
				: mc_libraries("")
			{
			}

			~Command() = default;

		public:
			void setJavaExcecutable(std::filesystem::path java_excecutable_path) { this->java_excecutable_path = java_excecutable_path; }
			void setJavaClassName(std::string java_class_name) { this->java_class_name = java_class_name; }
			void setJavaArguments(arguments::JavaArguments arguments) { this->java_arguments = arguments; }
			void setMCLibraries(libraries::Libraries libraries) { this->mc_libraries = libraries; }
			void setUserArguments(arguments::UserArguments arguments) { this->user_arguments = arguments; }

		public:
			const std::filesystem::path& getJavaExcecutable() const { return this->java_excecutable_path; }
			const std::string& getJavaClassName() const { return this->java_class_name; }
			const arguments::JavaArguments& getJavaArguments() const { return this->java_arguments; }
			const libraries::Libraries& getMCLibraries() const { return this->mc_libraries; }
			const arguments::UserArguments& getUserArguments() const { return this->user_arguments; }

		public:
			//Java argument
			void addJavaArgument(const utils::arguments::Argument& java_argument)
			{ 
				this->java_arguments.add(java_argument);
			}

			//Minecraft libraries
			void addMCLibrary(const libraries::SingleLibrary& minecraft_library)
			{
				this->mc_libraries.add(minecraft_library);
			}

			//User argument
			void addUserArgument(const utils::arguments::Argument& user_argument)
			{
				this->user_arguments.add(user_argument);
			}

		public:
			std::u32string toU32String(const std::u32string& string_separator = U" ") const;
			types::Vector<std::u32string> toU32StringVector() const;

		public:


		private:
			std::filesystem::path java_excecutable_path;
			arguments::JavaArguments java_arguments;
			//-cp
			libraries::Libraries mc_libraries;
			std::string java_class_name;
			arguments::UserArguments user_arguments;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_COMMAND_H_