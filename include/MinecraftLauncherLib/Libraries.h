#ifndef MINECRAFTLAUNCHERLIB_LIBRARIES_H_
#define MINECRAFTLAUNCHERLIB_LIBRARIES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <filesystem>

#include "MinecraftLauncherLib/SingleLibrary.h"
#include "MinecraftLauncherLib/Basic/MinecraftObject.h"
#include "MinecraftLauncherLib/Utils/Macros.h"
#include "MinecraftLauncherLib/Types/Vector.h"
#include "MinecraftLauncherLib/Rules.h"
#include "MinecraftLauncherLib/Basic/Natives.h"

namespace MCLCPPLIB_NAMESPACE
{
	class Libraries
	{
	public:
		Libraries() = default;
		~Libraries() = default;

	public:
		static Libraries fromJson(const nlohmann::json& json, const std::filesystem::path& path);
		
	public:
		const types::Vector<SingleLibrary>& getLibrariesPaths() const { return this->libraries_paths; }

	public:
		std::u32string toU32String() const
		{
			std::u32string string;

			for (auto& elem : libraries_paths)
			{
				string += elem.getLibraryPath().u32string() + std::u32string(1, CLASSPATH_SEPARATOR);
			}

			return string;
		}

	private:
		types::Vector<SingleLibrary> libraries_paths;
	};
}

#endif // !MINECRAFTLAUNCHERLIB_LIBRARIES_H_
