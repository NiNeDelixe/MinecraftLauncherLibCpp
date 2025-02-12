#pragma once
#ifndef DOWNLOAD_DOWNLOADMANAGER_H_
#define DOWNLOAD_DOWNLOADMANAGER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>

#include "MinecraftLauncherLib/Download/DownloadWorker.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace download
	{
		class DownloadManager
		{
		public:
			DownloadManager(const std::filesystem::path& downloads_path)
				: downloads_path(downloads_path)
			{
			}

			~DownloadManager() = default;

		private:
			std::filesystem::path downloads_path;

			DownloadWorker download_worker;
		};

	}
}

#endif // !DOWNLOAD_DOWNLOADMANAGER_H_
