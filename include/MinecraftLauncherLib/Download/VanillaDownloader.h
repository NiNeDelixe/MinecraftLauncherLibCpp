#pragma once
#ifndef DOWNLOAD_VANILLADOWNLOADER_H_
#define DOWNLOAD_VANILLADOWNLOADER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>
#include <stack>
#include <string>

#include <nlohmann/json.hpp>
#include <zip.h>

//#include "MinecraftLauncherLib/VersionProfile.h"
#include "MinecraftLauncherLib/UserOptions.h"

#include "MinecraftLauncherLib/Core/Rules.h"
#include "MinecraftLauncherLib/Core/SingleLibrary.h"
#include "MinecraftLauncherLib/Core/Libraries.h"

#include "MinecraftLauncherLib/Download/DownloadWorker.h"

#include "MinecraftLauncherLib/Utils/Interfaces/IDownloader.h"
#include "MinecraftLauncherLib/Utils/Archives/Archive.h"
#include "MinecraftLauncherLib/Utils/Delegates.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace archives
	{
		struct ExtractResult
		{
			std::string file_name;
			std::filesystem::path extraction_file;
		};

		class ExtractorWorker : public utils::threads::Worker<utils::delegates::runnable, ExtractResult>
		{
		public:
			ExtractorWorker(const std::filesystem::path& output_path)
				: output_path(output_path)
			{
			}

			~ExtractorWorker()
			{
			}

			ExtractResult operator()(const std::shared_ptr<utils::delegates::runnable>& archive) override
			{
				(*archive)();
				return ExtractResult();
			}

		private:
			std::filesystem::path output_path;
		};

	}

	namespace download
	{
		using namespace utils::callback;

		class VanillaDownloader : public utils::interfaces::IDownloader
		{
		public:
			VanillaDownloader(const std::string& version_id, const std::filesystem::path& downloads_directory,
				const options::RuntimeOptions& options, const utils::delegates::callback_supplier& callback);
			~VanillaDownloader() 
			{
				if (downloader_thread && downloader_thread->joinable()) 
				{
					downloader_thread->join();
				}
			};

		protected:
			void installVersion(const std::filesystem::path& url = "");

			bool extractNativesFiles(const std::filesystem::path& filename,
				const std::filesystem::path& extract_path, const nlohmann::json& extract_data);

			void installLibraries(const nlohmann::json& version_data) override;
			void installAssets(const nlohmann::json& version_data);
			virtual void doInstall() override;

		public:
			void installLoader() override;
			std::string getLatestInstallerVersion() override;


		public:
			void update()
			{
				download_thread_pool.update();
				extraction_thread_pool.update();
			}

		protected:
			std::map<std::string, utils::download::DownloadResult> results;
			std::string version_id;
			std::filesystem::path minecraft_path;
			std::filesystem::path downloads_directory;
			std::filesystem::path path_to_version_json;
			options::RuntimeOptions options;

			utils::delegates::callback_supplier callback;

			utils::threads::ThreadPool<DownloadWorker, utils::download::DownloadResult> download_thread_pool;
			utils::threads::ThreadPool<utils::delegates::runnable, archives::ExtractResult> extraction_thread_pool;

			std::shared_ptr<std::thread> downloader_thread = nullptr;
		};

	}
}

#endif // !DOWNLOAD_VANILLADOWNLOADER_H_
