#pragma once
#ifndef DOWNLOAD_FORGEDOWNLOADER_H_
#define DOWNLOAD_FORGEDOWNLOADER_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <filesystem>

#include "MinecraftLauncherLib/Core/Java.h"

#include "MinecraftLauncherLib/Download/VanillaDownloader.h"

#include "MinecraftLauncherLib/Utils/Archives/Archive.h"
#include "MinecraftLauncherLib/Utils/JavaUtils.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace download
	{
		namespace loaders
		{
			class ForgeDownloader : public VanillaDownloader
			{
			public:
				ForgeDownloader(const std::string& version_id, const std::string& forge_version,
					const std::filesystem::path& downloads_directory,
					const options::RuntimeOptions& options, const utils::delegates::callback_supplier& callback)
					: VanillaDownloader(version_id, downloads_directory, options, callback)
				{
					forge_version_comb = version_id + "-" + forge_version;
				}
				~ForgeDownloader() = default;

			public:
				//void installLoader() override;

			protected:
				void doInstall() override;
				void forgeProcessors(nlohmann::json& version_data, const std::filesystem::path& lzma_path,
					const std::filesystem::path& installer_path);

			private:
				std::string forge_version_comb;
			};
		}
	}
}

#endif // !DOWNLOAD_FORGEDOWNLOADER_H_
