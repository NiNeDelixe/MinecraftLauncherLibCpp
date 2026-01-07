#pragma once
#ifndef ARCHIVES_ARCHIVE_H_
#define ARCHIVES_ARCHIVE_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

#include <zip.h>

#include <nlohmann/json.hpp>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace archives
		{
			class MCLLIB_API Archive
			{
			public:
				explicit Archive(const std::filesystem::path& archive_path, const std::filesystem::path& output_file = "")
					: archive_path(archive_path), output_path(output_file)
				{
					int err = 0;
					archive = zip_open(archive_path.string().c_str(), ZIP_RDONLY, &err);
					if (!archive) 
					{
						zip_error_t error;
						zip_error_init_with_code(&error, err);
						std::string error_str(zip_error_strerror(&error));
						throw std::runtime_error("Failed to open archive: " + archive_path.string() + ", because: " + error_str);
					}
				}
				~Archive()
				{
					if (archive) 
					{
						zip_close(archive);
					}
				}

			public:
				std::vector<std::filesystem::path> getFileList() const
				{
					std::vector<std::filesystem::path> file_list;
					zip_int64_t num_files = zip_get_num_entries(archive, 0);
					for (zip_uint64_t i = 0; i < static_cast<zip_uint64_t>(num_files); ++i) 
					{
						struct zip_stat stat;
						zip_stat_index(archive, i, 0, &stat);
						file_list.emplace_back(stat.name);
					}
					return file_list;
				}

				void extractFile(const std::filesystem::path& file_name, const std::filesystem::path& output_path = "") const
				{
					std::filesystem::path final_output_path = output_path;
					if (final_output_path.empty())
					{
						final_output_path = this->output_path;
					}
					if (final_output_path.empty())
					{
						throw std::invalid_argument("output path is empty, add path in archive");
					}

					zip_file* zf = zip_fopen(archive, file_name.generic_string().c_str(), 0);
					if (!zf) 
					{
						throw std::runtime_error("Failed to open file in archive: " + file_name.string());
					}

					std::filesystem::create_directories(final_output_path.parent_path());

					std::ofstream out_file(final_output_path, std::ios::binary);
					char buffer[4096];
					zip_int64_t bytes_read;
					while ((bytes_read = zip_fread(zf, buffer, sizeof(buffer))) > 0) 
					{
						out_file.write(buffer, bytes_read);
					}

					zip_fclose(zf);
				}

				std::vector<uint8_t> getBytesFromFile(const std::filesystem::path& file_name)
				{
					zip_file* zf = zip_fopen(archive, file_name.generic_string().c_str(), 0);
					if (!zf)
					{
						zip_error_t* error = zip_get_error(archive);
						throw std::runtime_error("Failed to open file in archive: " + file_name.string() + ", because: " + zip_error_strerror(error));
					}

					std::vector<uint8_t> result;

					uint8_t buffer[4096];
					zip_int64_t bytes_read;
					while ((bytes_read = zip_fread(zf, buffer, sizeof(buffer))) > 0)
					{
						for (size_t i = 0; i < bytes_read; i++)
						{
							result.emplace_back(buffer[i]);
						}
					}

					zip_fclose(zf);

					return result;
				}

				void extractAll(const std::filesystem::path& output_directory = "") const
				{
					std::filesystem::path final_output_path;
					if (output_directory.empty())
					{
						final_output_path = this->output_path;
					}
					if (final_output_path.empty())
					{
						throw std::invalid_argument("output path is empty, add path in archive");
					}

					if (!std::filesystem::exists(final_output_path))
					{
						std::filesystem::create_directories(final_output_path);
					}

					zip_int64_t num_files = zip_get_num_entries(archive, 0);

					for (zip_uint64_t i = 0; i < static_cast<zip_uint64_t>(num_files); ++i) 
					{
						struct zip_stat stat;
						zip_stat_index(archive, i, 0, &stat);

						std::string file_name = stat.name;
						std::filesystem::path output_path = final_output_path / file_name;

						if (file_name.back() == '/') 
						{
							std::filesystem::create_directories(output_path);
						}
						else 
						{
							extractFile(file_name, output_path);
						}
					}
				}

			private:
				std::filesystem::path archive_path;
				std::filesystem::path output_path;
				zip* archive = nullptr;
			};

		}
	}
}

#endif // ARCHIVES_ARCHIV_H_