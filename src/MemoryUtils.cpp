#include "MinecraftLauncherLib/Utils/MemoryUtils.h"

std::wstring MCLCPPLIB_NAMESPACE::utils::convert::convertStringToWstring(const std::string& conv)
{
    /*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(conv);
    return wide;*/
    return boost::locale::conv::template to_utf<wchar_t>(conv, "UTF-8");
}
