#pragma once
#ifndef MINECRAFTLAUNCHERLIB_JAVA_H_
#define MINECRAFTLAUNCHERLIB_JAVA_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <regex>
#include <sstream>

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Utils/Macros.h"
#include "MinecraftLauncherLib/Utils/StringUtils.h"
#include "MinecraftLauncherLib/Utils/Archives/Archive.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace java
	{
		class MCLLIB_API Java
		{
		public:
			Java() = default;
			~Java() = default;

			static std::filesystem::path getExecutablePath(const std::string& jvm_version, const std::filesystem::path& minecraft_directory);

			template<class STR>
			static STR getJarMainclass(const std::filesystem::path& path)
			{
				utils::archives::Archive archive(path);

				auto& manifest_content = archive.getBytesFromFile("META-INF\\MANIFEST.MF");

				STR manifest_str(manifest_content.begin(), manifest_content.end());

				std::basic_istringstream<STR::value_type, std::char_traits<STR::value_type>, std::allocator<STR::value_type>> manifest_stream(manifest_str);
				std::vector<STR> lines;
				STR line{};

				while (std::getline(manifest_stream, line)) 
				{
					if (!line.empty() && line.back() == '\r') 
					{
						line.pop_back();
					}
					lines.push_back(line);
				}

				STR key{};
				STR value{};
				std::unordered_map<STR, STR> content;
				for (auto& line : lines)
				{
					auto parts = MCLCPPLIB_NAMESPACE::utils::string::split(line, ':');

					if (parts.size() > 1)
					{
						key = parts[0];
						value = parts[1];

						content.emplace(key, value.substr(1));
					}
				}

				return content[STR({ 0x4D, 0x61, 0x69, 0x6E, 0x2D, 0x43, 0x6C, 0x61, 0x73, 0x73 })];
			}

		private:
			static std::string getJvmPlatformString();

		private:

		};


	}
}

#endif //MINECRAFTLAUNCHERLIB_JAVA_H_