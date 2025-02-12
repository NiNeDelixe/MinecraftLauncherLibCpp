#include "MinecraftLauncherLib/Download/DownloadWorker.h"

CURLcode MCLCPPLIB_NAMESPACE::download::DownloadWorker::download(const std::filesystem::path& url, const std::filesystem::path& download_folder)
{
    std::filesystem::path final_folder;
    if (download_folder.empty())
    {
        final_folder = this->download_folder;
    }
    std::filesystem::path final_url;
    if (url.empty())
    {
        final_url = this->url;
    }
    if (final_url.empty())
    {
        throw std::invalid_argument("url is empty, give url to worker or in download function");
    }

    if (std::filesystem::is_directory(final_folder) || final_folder.filename().empty())
    {
        final_folder /= final_url.filename();
    }
    this->file_path = final_folder;
    
    std::filesystem::create_directories(final_folder.parent_path());


    std::ofstream outFile(final_folder, std::ios::binary | std::ios::trunc);

    if (!outFile.is_open()) 
    {
        //std::cerr << "Не удалось открыть файл для записи" << std::endl;
        return CURLcode(23);
    }

    utils::download::CallbackHelper callback_helper;
    callback_helper.file = &outFile;
    callback_helper.callback = callback;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl;
    CURLcode res{};
    curl = curl_easy_init();

    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, final_url.string().c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &DownloadWorker::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callback_helper);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    callback_helper.file->close();
    return res;
}

MCLCPPLIB_NAMESPACE::types::Vector<CURLcode> MCLCPPLIB_NAMESPACE::download::DownloadWorker::download(const types::Vector<std::filesystem::path>& urls, const std::filesystem::path& download_folder) const
{

    return types::Vector<CURLcode>();
}
