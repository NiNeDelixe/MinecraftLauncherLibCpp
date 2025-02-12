#pragma once
#ifndef DOWNLOAD_FABRICDOWNLOADER_H_
#define DOWNLOAD_FABRICDOWNLOADER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>

#include "MinecraftLauncherLib/Download/DownloadWorker.h"

#include "MinecraftLauncherLib/Download/VanillaDownloader.h"

#include "MinecraftLauncherLib/VersionProfile.h"

//#include "MinecraftLauncherLib/Utils/Archives/Archive.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace download
	{
		namespace loaders
		{
			class FabricDownloader : public VanillaDownloader
			{
			public:
				FabricDownloader(const std::string& version_id, const std::string& fabric_version,
					const std::filesystem::path& downloads_directory,
					const options::RuntimeOptions& options, const utils::delegates::callback_supplier& callback)
					: VanillaDownloader(version_id, downloads_directory, options, callback), fabric_loader_version(fabric_version)
				{
					
				}
				~FabricDownloader() = default;

			protected:
				void doInstall() override;

			public:
				//void installLoader() override;

			private:
				std::string fabric_loader_version;
			};
		}
	}
}

#endif // !DOWNLOAD_FABRICDOWNLOADER_H_
