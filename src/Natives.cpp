#include <MinecraftLauncherLib/Basic/Natives.h>

std::string MCLCPPLIB_NAMESPACE::natives::Natives::getNativesFromJson(const nlohmann::json& data)
{
	/*
	Returns the native part from the json data
	*/
	std::string arch_type;
	if (ARCH == "x86")
	{
		arch_type = "32";
	}
	else
	{
		arch_type = "64";
	}

	if (data.contains("natives"))
	{
		if (OS == "windows")
		{
			if (data["natives"].contains("windows"))
			{
				std::string replace_string = data["natives"]["windows"].template get<std::string>();
				MCLCPPLIB_NAMESPACE::utils::string::replace(replace_string, std::string("${arch}"), arch_type);
				return replace_string;
			}
			else
			{
				return "";
			}
		}
		else if (OS == "mac")
		{
			if (data["natives"].contains("osx"))
			{
				std::string replace_string = data["natives"]["osx"].template get<std::string>();
				MCLCPPLIB_NAMESPACE::utils::string::replace(replace_string, std::string("${arch}"), arch_type);
				return replace_string;
			}
			else
			{
				return "";
			}
		}
		else
		{
			if (data["natives"].contains("linux"))
			{
				std::string replace_string = data["natives"]["linux"].template get<std::string>();
				MCLCPPLIB_NAMESPACE::utils::string::replace(replace_string, std::string("${arch}"), arch_type);
				return replace_string;
			}
			else
			{
				return "";
			}
		}
	}
	else
	{
		return "";
	}
}