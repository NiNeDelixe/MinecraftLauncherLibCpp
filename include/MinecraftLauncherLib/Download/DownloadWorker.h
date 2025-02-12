#pragma once
#ifndef DOWNLOAD_DOWNLOADWORKER_H_
#define DOWNLOAD_DOWNLOADWORKER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>
#include <fstream>

#include <curl/curl.h>

#include "MinecraftLauncherLib/Utils/ThreadPool.h"
#include "MinecraftLauncherLib/Utils/Delegates.h"

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace download
		{
			struct DownloadResult
			{
				std::string file_name;
				CURLcode code{};
				std::filesystem::path file_path;
			};

			struct CallbackHelper
			{
				utils::delegates::callback_supplier callback = nullptr;
				std::ofstream* file = nullptr;
			};
		}
	}

	namespace download
	{
		class DownloadWorker : public utils::threads::Worker<DownloadWorker, utils::download::DownloadResult>
		{
		public:
			DownloadWorker(const std::filesystem::path& download_folder, const std::filesystem::path& url = "", const utils::delegates::callback_supplier& callback = nullptr)
				: download_folder(download_folder), url(url), callback(callback) {};
			~DownloadWorker() = default;

		public:
			CURLcode download(const std::filesystem::path& url = "", const std::filesystem::path& download_folder = "");

			types::Vector<CURLcode> download(const types::Vector<std::filesystem::path>& urls,
				const std::filesystem::path& download_folder = "") const;

			utils::download::DownloadResult operator()(const std::shared_ptr<DownloadWorker>& downloader) override
			{
				CURLcode code = downloader->download();
				return utils::download::DownloadResult{ downloader->url.filename().string() , code, downloader->download_folder};
			}

			static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
			{
				size_t current = (size * nmemb);
				utils::download::CallbackHelper* helper = static_cast<utils::download::CallbackHelper*>(userp);
				if (helper->callback)
				{
					helper->callback(&size, &current, nullptr, nullptr, nullptr);
				}
				helper->file->write(static_cast<char*>(contents), current);
				return current;
			}

		public:
			const std::filesystem::path& getDownloadFolder() { return this->download_folder; }
			const std::filesystem::path& getFilePath() { return this->file_path; }

		private:
			std::filesystem::path download_folder;
			std::filesystem::path url;
			std::filesystem::path file_path;

			utils::delegates::callback_supplier callback;
		};

	}
}

#endif // !DOWNLOAD_DOWNLOADWORKER_H_
