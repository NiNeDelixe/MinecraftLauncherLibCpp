#include "MinecraftLauncherLib/Core/Command.h"

std::u32string MCLCPPLIB_NAMESPACE::command::Command::toU32String(const std::u32string& string_separator) const
{
	std::u32string result;
	result += java_excecutable_path.u32string();
	result += string_separator;
	for (auto& java_argument : java_arguments.toU32StringsVector())
	{
		result += java_argument + string_separator;
	}
	result += string_separator;
	result += U"-cp";
	result += string_separator;
	for (auto& library : mc_libraries.toPaths())
	{
		result += library.u32string() + std::u32string(CLASSPATH_SEPARATOR);
	}
	result += string_separator;
	result += std::filesystem::path(java_class_name).u32string(); //FIXME: change to normal converter
	result += string_separator;
	for (auto& user_argument : user_arguments.toU32StringsVector())
	{
		result += user_argument + string_separator;
	}
	return result;
}

MCLCPPLIB_NAMESPACE::types::Vector<std::u32string> MCLCPPLIB_NAMESPACE::command::Command::toU32StringVector() const
{
	types::Vector<std::u32string> result;
	result.push_back(java_excecutable_path.u32string());
	for (auto& java_argument : java_arguments.toU32StringsVector())
	{
		result.push_back(java_argument);
	}
	result.push_back(U"-cp");
	for (auto& library : mc_libraries.toPaths())
	{
		result.push_back(library.u32string());
	}
	result.push_back(std::filesystem::path(java_class_name).u32string()); //FIXME: change to normal converter
	for (auto& user_argument : user_arguments.toU32StringsVector())
	{
		result.push_back(user_argument);
	}
	return result;
}
