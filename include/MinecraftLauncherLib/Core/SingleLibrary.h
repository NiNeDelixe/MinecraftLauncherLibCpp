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

				std::filesystem::path download_url = "";

				std::vector<STR> parts = MCLCPPLIB_NAMESPACE::utils::string::split(name, ':');
				STR& base_path = parts[0];
				STR& libname = parts[1];
				STR& version = parts[2];
				STR extra{};
				STR fileend{};

				for (auto& var : MCLCPPLIB_NAMESPACE::utils::string::split(base_path, '.'))
				{
					libpath /= var.c_str();
					download_url /= var.c_str();
				}

				//STR::value_type('@')
				STR::value_type version_spliter = STR::value_type(0x40);

				if (version.find(version_spliter) != STR::npos && version[version.find(version_spliter)] == version_spliter)
				{
					std::vector<STR> version_split = MCLCPPLIB_NAMESPACE::utils::string::split(version, version_spliter, 2);
					version = version_split[0];
					fileend = version_split[1];
				} 
				else if (parts.size() > 3) 
				{
					extra = parts[3];

					if (MCLCPPLIB_NAMESPACE::utils::string::split(extra, '@').size() <= 1)
					{
						if (extra == STR({ 0x6D, 0x61, 0x70, 0x70, 0x69, 0x6E, 0x67, 0x73 }) ||
							extra == STR({ 0x6D, 0x61, 0x70, 0x70, 0x69, 0x6E, 0x67, 0x73, 0x2D, 0x6D, 0x65, 0x72, 0x67, 0x65, 0x64 }))
						{
							fileend = STR({ 0x74, 0x78, 0x74 });
						}
						else
						{
							fileend = STR({ 0x6A, 0x61, 0x72 });
						}
					}
					else
					{
						auto splitted_extra = MCLCPPLIB_NAMESPACE::utils::string::split(extra, '@');

						extra = splitted_extra[0];
						fileend = splitted_extra[1];
					}
				}
				else
				{
					//STR("jar")
					fileend = STR({ 0x6A, 0x61, 0x72 });
				}
				// construct a filename with the remaining parts

				//STR("-")
				STR filename = libname + STR({ 0x2D }) + version;
				if (!extra.empty()) 
				{
					filename += STR({ 0x2D }) + extra;
				}
				else
				{
					for (size_t i = 3; i < parts.size(); ++i)
					{
						//STR("-")
						filename += STR({ 0x2D }) + parts[i];
					}
				}
				//STR(".")
				filename += STR({ 0x2E }) + fileend;

				libpath = libpath / libname / version / filename;
				download_url = download_url / libname / version / filename;

				SingleLibrary lib(libpath);
				lib.library_url = download_url;
				lib.library_name = std::filesystem::path(libname).string();
				lib.library_version = std::filesystem::path(version).string();
				return lib;
			}

			//template<typename STR>
			//static SingleLibrary getFrogeLibraryFromName(const STR& name, const std::filesystem::path& path)
			//{
			//	std::filesystem::path libpath = path / "libraries";

			//	STR original_libname = name;
			//	original_libname = original_libname.substr(1, original_libname.size() - 2);

			//	std::filesystem::path download_url = "";

			//	std::vector<STR> parts = MCLCPPLIB_NAMESPACE::utils::string::split(original_libname, ':');
			//	STR& base_path = parts[0];
			//	STR& libname = parts[1];
			//	STR& version = parts[2];
			//	STR& extra = parts[3];
			//	STR fileend;

			//	//STR::value_type('@')
			//	STR::value_type version_spliter = STR::value_type(0x40);

			//	for (auto& var : MCLCPPLIB_NAMESPACE::utils::string::split(base_path, '.'))
			//	{
			//		libpath /= var.c_str();
			//		download_url /= var.c_str();
			//	}
			//	if (version.find(version_spliter) != STR::npos && version[version.find(version_spliter)] == version_spliter)
			//	{
			//		std::vector<STR> splt = MCLCPPLIB_NAMESPACE::utils::string::split(version, version_spliter, 2);
			//		if (splt.size() <= 1)
			//		{					
			//			if (extra == STR({ 0x6D, 0x61, 0x70, 0x70, 0x69, 0x6E, 0x67, 0x73 }) || 
			//				extra == STR({ 0x6D, 0x61, 0x70, 0x70, 0x69, 0x6E, 0x67, 0x73, 0x2D, 0x6D, 0x65, 0x72, 0x67, 0x65, 0x64 }))
			//			{
			//				fileend = "txt";
			//			}
			//			else
			//			{
			//				fileend = "jar";
			//			}
			//		}
			//		else
			//		{
			//			version = splt[0];
			//			fileend = splt[1];
			//		}
			//	}
			//	else
			//	{
			//		//STR("jar")
			//		fileend = STR({ 0x6A, 0x61, 0x72 });
			//	}
			//	// construct a filename with the remaining parts

			//	//STR("-")
			//	STR filename = libname + STR({ 0x2D }) + version + STR({ 0x2D }) + extra;

			//	for (size_t i = 3; i < parts.size(); ++i)
			//	{
			//		//STR("-")
			//		filename += STR({ 0x2D }) + parts[i];
			//	}
			//	//STR(".")
			//	filename += STR({ 0x2E }) + fileend;

			//	libpath = libpath / libname / version / filename;
			//	download_url = download_url / libname / version / filename;

			//	SingleLibrary lib(libpath);
			//	lib.library_url = download_url;
			//	lib.library_name = std::filesystem::path(libname).string();
			//	lib.library_version = std::filesystem::path(version).string();
			//	return lib;
			//}

		public:
			const std::filesystem::path& getLibraryPath() const { return this->library_path; }
			const std::filesystem::path& getLibraryUrl() const { return this->library_url; }
			const std::string& getLibraryName() const { return this->library_name; }
			const std::string& getLibraryVersion() const { return this->library_version; }

		private:
			std::filesystem::path library_path;
			std::filesystem::path library_url;

			std::string library_name;
			std::string library_version;
		};
	}
}

#endif // !MINECRAFTLAUNCHERLIB_SINGLELIBRARY_H_
