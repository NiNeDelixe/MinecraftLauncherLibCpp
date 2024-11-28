#ifndef MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_
#define MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <type_traits>

#include "MinecraftLauncherLib/Utils/StringUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace libraries
	{
		class SingleLibrary
		{
		public:
			SingleLibrary(const std::filesystem::path& lib_path);
			~SingleLibrary() = default;

		public:
			/// <summary>
			/// Returns the path from a libname
			/// </summary>
			/// <typeparam name="STR">Standart string</typeparam>
			/// <param name="name">Name of library</param>
			/// <param name="path">Path to minecraft directory</param>
			/// <returns>Path from a libname</returns>
			template<typename STR>
			static SingleLibrary getLibraryFromName(const STR& name, const std::filesystem::path& path)
			{
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
					//STR("jar")
					fileend = STR({ 0x6A, 0x61, 0x72 });
				}
				// construct a filename with the remaining parts

				//STR("-")
				STR filename = libname + STR({ 0x2D }) + version;

				for (size_t i = 3; i < parts.size(); ++i)
				{
					//STR("-")
					filename += STR({ 0x2D }) + parts[i];
				}
				//STR(".")
				filename += STR({ 0x2E }) + fileend;

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
}

#endif // !MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_
