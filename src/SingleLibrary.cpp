#include "MinecraftLauncherLib/SingleLibrary.h"

MCLCPPLIB_NAMESPACE::libraries::SingleLibrary::SingleLibrary(const std::filesystem::path& lib_path)
	: library_path(lib_path)
{
}
