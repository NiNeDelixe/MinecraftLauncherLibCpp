#ifndef MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_
#define MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>

#include "MinecraftLauncherLib/Utils/StringUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	class SingleLibrary
	{
	public:
		SingleLibrary(const std::filesystem::path& lib_path);
		~SingleLibrary() = default;

	public:
		template<typename STR>
		static SingleLibrary getLibraryFromName(const STR& name, const std::filesystem::path& path)
		{
			/*
			Returns the path from a libname
			*/

			std::filesystem::path libpath = path / "libraries";

			std::vector<STR> parts = MCLCPPLIB_NAMESPACE::utils::string::split(name, ':');
			STR& base_path = parts[0];
			STR& libname = parts[1];
			STR& version = parts[2];
			STR fileend;

			char varsion_splitter_char = '@';
			STR::value_type version_spliter = static_cast<STR::value_type>(varsion_splitter_char);

			for (auto& var : MCLCPPLIB_NAMESPACE::utils::string::split(base_path, '.'))
			{
				libpath /= var.c_str();
			}
			if (version.find(version_spliter) != STR::npos && version[version.find(version_spliter)] == version_spliter)
			{
				std::vector<STR> splt = MCLCPPLIB_NAMESPACE::utils::string::split(version, version_spliter, 2);
				version = splt[0];
				fileend = splt[1];
			}
			else
			{
				fileend = static_cast<STR>(std::string("jar"));
			}
			// construct a filename with the remaining parts

			STR filename = libname + static_cast<STR>(std::string("-")) + version;

			for (size_t i = 3; i < parts.size(); ++i)
			{
				filename += static_cast<STR>(std::string("-")) + parts[i];
			}
			filename += static_cast<STR>(std::string(".")) + fileend;

			libpath = libpath / libname / version / filename;

			SingleLibrary lib(libpath);
			return lib;
		}

	public:
		const std::filesystem::path& getLibraryPath() const { return this->library_path; }

	private:
		std::filesystem::path library_path;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_
