#include "MinecraftLauncherLib/Download/FabricDownloader.h"

void MCLCPPLIB_NAMESPACE::download::loaders::FabricDownloader::doInstall()
{
	std::string loader_version = this->fabric_loader_version;
	std::filesystem::path minecraft_directory = this->options.minecraft_directory.get();
	std::string fabric_minecraft_version = "fabric-loader-" + loader_version + "-" + this->version_id;
	std::filesystem::path fabric_version_dir = minecraft_directory / "versions" / fabric_minecraft_version;


	VanillaDownloader::installLoader();

	std::filesystem::create_directories(fabric_version_dir);

	// Get installer version
	//std::string installer_version = get_latest_installer_version();

	std::string libraries_url = "https://meta.fabricmc.net/v2/versions/loader/"
		+ this->version_id + "/" + loader_version + "/profile/json";

	std::filesystem::path url = libraries_url;
	std::filesystem::path dw_path = fabric_version_dir / (fabric_minecraft_version + ".json");

	this->download_thread_pool.enqueueJobWithPriority(std::make_shared<DownloadWorker>(dw_path, url),
		"fabric_" + loader_version + "_version_json");

	this->download_thread_pool.waitForPriorityJob("fabric_" + loader_version + "_version_json");

	std::ifstream ifst(dw_path);
	nlohmann::json version_data = nlohmann::json::parse(ifst);
	ifst.close();

	version_data = VersionProfile::inheritJson(version_data, minecraft_directory);
	installLibraries(version_data);
	//MinecraftCpp::natives::downloadNatives(fabric_version_dir / "natives", callback);

	if (!std::filesystem::is_directory(this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / (version_data["id"].template get<std::string>() + ".jar")) && version_data.contains("inheritsFrom"))
	{
		std::filesystem::copy_file
		(
			this->minecraft_path / "versions" / version_data["inheritsFrom"].template get<std::string>() / (version_data["inheritsFrom"].template get<std::string>() + ".jar"),
			this->minecraft_path / "versions" / version_data["id"].template get<std::string>() / (version_data["id"].template get<std::string>() + ".jar"),
			std::filesystem::copy_options::overwrite_existing
		);
	}
}

//void MCLCPPLIB_NAMESPACE::download::loaders::FabricDownloader::installLoader()
//{
//	return doInstall();
//}