#pragma once
#ifndef INTERFACES_IDOWNLOADER_H_
#define INTERFACES_IDOWNLOADER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <nlohmann/json.hpp>

#include "MinecraftLauncherLib/Download/DownloadWorker.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace interfaces
		{
			/// <summary>
			/// Interface for loader downloaders
			/// </summary>
			class IDownloader
			{
			public:
				virtual ~IDownloader() {};

				virtual void installLoader() = 0;
				virtual std::string getLatestInstallerVersion() = 0;

			public:
				virtual void doInstall() = 0;
				virtual void installLibraries(const nlohmann::json& version_data) = 0;
			};
		}
	}
}

#endif // INTERFACES_IDOWNLOADER_H_