#include "MinecraftLauncherLib/Arguments.h"

#include "MinecraftLauncherLib/VersionProfile.h"

MCLCPPLIB_NAMESPACE::arguments::BasicArguments MCLCPPLIB_NAMESPACE::arguments::BasicArguments::fromJson(
	const nlohmann::json& data, const nlohmann::json& version_data, const std::filesystem::path& minecraft_path,
	const UserProfile& user_options, const VersionProfile& profile_options)
{
	BasicArguments arguments;

	std::u32string key;
	std::u32string value;

	for (auto& var : data)
	{
		if (var.type() == nlohmann::json::value_t::string)
		{
			auto s = var.template get<std::filesystem::path>();
			replaceArgument(s.u32string(), version_data, minecraft_path, user_options, profile_options);
			if (s.u32string()._Starts_with(U"--"))
			{
				key = s.u32string();
			}
			else
			{
				value = s.u32string();
				utils::arguments::Argument rep(key, value);
				key = U"";
				value = U"";
				arguments.arguments.push_back(rep);
			}
		}
		else
		{
			// Rules might has 2 different names in different versions.json
			if (var.contains("compatibilityRules") && !rules::Rules::parseRuleList(var, "compatibilityRules"))
			{
				continue;
			}
			if (var.contains("rules") && !rules::Rules::parseRuleList(var, "rules"))
			{
				continue;
			}

			// var could be the argument
			if (var["value"].type() == nlohmann::json::value_t::string)
			{
				auto a = var.template get<std::filesystem::path>();
				replaceArgument(a.u32string(), version_data, minecraft_path, user_options, profile_options);
				
				if (a.u32string()._Starts_with(U"--"))
				{
					key = a.u32string();
				}
				else
				{
					value = a.u32string();
					utils::arguments::Argument replace(key, value);
					key = U"";
					value = U"";
					arguments.arguments.push_back(replace);
				}
			}
			else
			{
				for (auto& v : var["value"])
				{
					utils::arguments::Argument val;
					if (v["value"].type() == nlohmann::json::value_t::array)
					{
						auto a = v["value"][0].template get<std::filesystem::path>();
						replaceArgument(a.u32string(), version_data, minecraft_path, user_options, profile_options);
						if (a.u32string()._Starts_with(U"--"))
						{
							key = a.u32string();
						}
						else
						{
							value = a.u32string();
							val = utils::arguments::Argument(key, value);
							key = U"";
							value = U"";
						}
					}
					else
					{
						auto a = v["value"].template get<std::filesystem::path>();
						replaceArgument(a.u32string(), version_data, minecraft_path, user_options, profile_options);
						if (a.u32string()._Starts_with(U"--"))
						{
							key = a.u32string();
						}
						else
						{
							value = a.u32string();
							val = utils::arguments::Argument(key, value);
							key = U"";
							value = U"";
						}
					}

					arguments.arguments.push_back(val);
				}
			}
		}
	}

	return arguments;
}

MCLCPPLIB_NAMESPACE::arguments::BasicArguments MCLCPPLIB_NAMESPACE::arguments::BasicArguments::fromJson(
	const nlohmann::json& version_data, const std::filesystem::path& minecraft_path,
	const UserProfile& user_options, const VersionProfile& profile_options)
{
	/*
	Turns the argument string from the version.json into a list
	*/
	BasicArguments arglist;

	for (auto& var : utils::string::split(version_data["minecraftArguments"].template get<std::string>(), ' '))
	{
		//replaceArgument(var, version_data, minecraft_path, user_options, profile_options);
		//arglist.arguments.push_back(utils::arguments::Argument(var));
	}
	// Custom resolution is not in the list
	if (profile_options.getUserOptions().customResolution)
	{
		//arglist.arguments.push_back(utils::arguments::Argument(U"--width", std::to_string(profile_options.getUserOptions().resolutionWidth)));
		//arglist.arguments.push_back(utils::arguments::Argument(U"--height", std::to_string(profile_options.getUserOptions().resolutionHeight)));
	}
	if (profile_options.getUserOptions().demo)
	{
		arglist.arguments.push_back(utils::arguments::Argument(U"--demo", std::u32string(U"true")));
	}

	return arglist;
}
