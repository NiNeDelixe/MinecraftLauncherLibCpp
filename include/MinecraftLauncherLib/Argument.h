#ifndef MINECRAFTLAUNCHERLIB_ARGUMENT_H_
#define MINECRAFTLAUNCHERLIB_ARGUMENT_H_

#include "MCLLib-api.h"

#include <variant>
#include <filesystem>
#include <string>
#include <vector>
#include <sstream>

#include "MinecraftLauncherLib/Types/Vector.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace arguments
		{
			class Argument
			{
			public:
				Argument(std::u32string key, std::variant<std::u32string, std::filesystem::path> value)
					: key(key), value(value) {}
				Argument(std::variant<std::u32string, std::filesystem::path> value)
					: Argument(U"", value) {}
				Argument() = default;
				~Argument() = default;

			public:
				const std::u32string& getKey() const { return this->key; }
				const std::variant<std::u32string, std::filesystem::path>& getValue() const { return this->value; }

			public:
				std::u32string toU32String() const
				{
					std::u32string string;

					string += key;
					string += U" ";

					try
					{
						string += U"\"" + std::get<std::u32string>(value) + U"\"";
					}
					catch (const std::bad_variant_access&)
					{
						string += U"\"" + std::get<std::filesystem::path>(value).u32string() + U"\"";
					}

					return string;
				}

			private:
				std::u32string key = U"";
				std::variant<std::u32string, std::filesystem::path> value = std::variant<std::u32string, std::filesystem::path>();
			};
		}
	}
}

#endif // !MINECRAFTLAUNCHERLIB_ARGUMENT_H_
