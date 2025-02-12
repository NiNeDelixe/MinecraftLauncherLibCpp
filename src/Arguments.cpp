#include "MinecraftLauncherLib/Core/Arguments.h"

MCLCPPLIB_NAMESPACE::arguments::Arguments MCLCPPLIB_NAMESPACE::arguments::Arguments::getArgumentsFromJson(const nlohmann::json& data, 
	const nlohmann::json& versionData, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths)
{
	/*
	Returns all arguments from the version.json
	*/

	Arguments arguments;
	auto& arglist = arguments.arguments;

	std::u32string previous;
	for (const auto& var : data)
	{
		if (var.type() != nlohmann::json::value_t::string)
		{
			continue;
		}
		

		processValue(var, previous, arglist, versionData, profile, libraries_paths);
	}

	std::u32string big_previous;
	for (const auto& var : data)
	{
		if (var.type() == nlohmann::json::value_t::string)
		{
			continue;
		}

		// Rules might has 2 different names in different versions.json
		//FIXME: add options
		if (var.contains("compatibilityRules") && !rules::Rules::parseRuleList(var, "compatibilityRules"/*, options*/))
		{
			continue;
		}
		if (var.contains("rules") && !rules::Rules::parseRuleList(var, "rules"/*, options*/))
		{
			continue;
		}

		// var could be the argument
		if (var["value"].type() == nlohmann::json::value_t::string) 
		{
			processValue(var["value"], big_previous, arglist, versionData, profile, libraries_paths);
		}
		else 
		{
			std::u32string small_previous;
			for (const auto& v : var["value"]) 
			{
				const auto& value = (v.type() == nlohmann::json::value_t::array) ? v[0] : v;
				processValue(value, small_previous, arglist, versionData, profile, libraries_paths);
			}
		}
	}

	return arguments;
}

std::u32string MCLCPPLIB_NAMESPACE::arguments::Arguments::replaceArguments(const std::u32string& replace_to, 
	const nlohmann::json& data, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths)
{
	std::u32string replace = replace_to;

	auto& options = profile.getOptions();
	auto& username = profile.getUsername();
	auto& uuid = profile.getUuid();
	auto& token = profile.getAccessToken();
	auto& xuid = profile.getXuid();
	auto& minecraft_path = options.minecraft_directory.get();

	std::u32string s;
	if (replace.find(U"${classpath}") != std::u32string::npos)
	{
		for (const auto& piece : libraries_paths) 
			s += piece.u32string() + std::u32string(CLASSPATH_SEPARATOR);
	}

	utils::string::replace(replace, std::u32string(U"${natives_directory}"), options.natives_directory.get().u32string());
	utils::string::replace(replace, std::u32string(U"${launcher_name}"), options.launcher_name.get());
	utils::string::replace(replace, std::u32string(U"${launcher_version}"), options.launcher_version.get());
	utils::string::replace(replace, std::u32string(U"${classpath}"), s);
	utils::string::replace(replace, std::u32string(U"${auth_player_name}"), !username.empty() ? username : std::u32string(U"{username}"));
	utils::string::replace(replace, std::u32string(U"${version_name}"), data["id"].template get<std::string>());
	utils::string::replace(replace, std::u32string(U"${game_directory}"), minecraft_path.u32string());
	utils::string::replace(replace, std::u32string(U"${assets_root}"), (options.assets_directory.get() / "assets").u32string());
	utils::string::replace(replace, std::u32string(U"${assets_index_name}"), data.contains("assets") ? data["assets"].template get<std::string>() : data["id"].template get<std::string>());
	utils::string::replace(replace, std::u32string(U"${auth_uuid}"), uuid.empty() ? std::u32string(U"{uuid}") : uuid);
	utils::string::replace(replace, std::u32string(U"${auth_access_token}"), token.empty() ? std::u32string(U"{token}") : token);
	utils::string::replace(replace, std::u32string(U"${user_type}"), std::u32string(U"mojang"));
	utils::string::replace(replace, std::u32string(U"${version_type}"), data["type"].template get<std::string>());
	utils::string::replace(replace, std::u32string(U"${user_properties}"), std::u32string(U"{}"));
	utils::string::replace(replace, std::u32string(U"${resolution_width}"), std::to_string(options.custom_resolution_width.get()));
	utils::string::replace(replace, std::u32string(U"${resolution_height}"), std::to_string(options.custom_resolution_height.get()));
	utils::string::replace(replace, std::u32string(U"${game_assets}"), (minecraft_path / "assets" / "virtual" / "legacy").u32string());
	utils::string::replace(replace, std::u32string(U"${auth_session}"), token.empty() ? std::u32string(U"{token}") : token);
	utils::string::replace(replace, std::u32string(U"${library_directory}"), (options.libraries_directory.get() / "libraries").u32string());
	utils::string::replace(replace, std::u32string(U"${classpath_separator}"), std::u32string(CLASSPATH_SEPARATOR));
	//FIXME: add options below in options class
	utils::string::replace(replace, std::u32string(U"${quickPlayPath}"), std::u32string(U"quickPlayPath"));
	utils::string::replace(replace, std::u32string(U"${quickPlaySingleplayer}"), std::string("quickPlaySingleplayer"));
	utils::string::replace(replace, std::u32string(U"${quickPlayMultiplayer}"), std::string("quickPlayMultiplayer"));
	utils::string::replace(replace, std::u32string(U"${quickPlayRealms}"), std::string("quickPlayRealms"));

	return replace;
}

MCLCPPLIB_NAMESPACE::types::Vector<std::u32string> MCLCPPLIB_NAMESPACE::arguments::Arguments::toU32StringsVector() const
{
	types::Vector<std::u32string> vector;
	
	for (auto& el : this->arguments)
	{
		vector.push_back(el.key);
		if (el.value.value_or(U"").empty())
		{
			continue;
		}
		vector.push_back(el.value.value_or(U""));
	}
	
	return vector;
}

void MCLCPPLIB_NAMESPACE::arguments::Arguments::processValue(const nlohmann::json& value, std::u32string& previous,
	types::Vector<utils::arguments::Argument>& arglist, const nlohmann::json& versionData, const UserProfile& profile, const types::Vector<std::filesystem::path>& libraries_paths)
{
	std::u32string original = value.template get<std::filesystem::path>().u32string();
	std::u32string replace = replaceArguments(original, versionData, profile, libraries_paths);

	//arglist.push_back(utils::arguments::Argument(replace, {}));

	if (replace._Starts_with(U"-")) {
		// Если уже есть флаг без значения, добавляем его
		if (!previous.empty()) {
			arglist.push_back(utils::arguments::Argument(previous, {}));
		}
		// Запоминаем новый флаг
		previous = replace;
	}
	else {
		// Если `previous` содержит флаг, связываем его с `replace`
		if (!previous.empty()) {
			arglist.push_back(utils::arguments::Argument(previous, replace));
			previous.clear();
		}
		else {
			// Если `previous` пуст, просто добавляем аргумент без пары
			arglist.push_back(utils::arguments::Argument(replace, {}));
		}
	}

	/*if (replace._Starts_with(U"-") && previous.empty())
	{
		previous = replace;
	}
	else
	{
		if (!previous.empty() && original != replace && !replace._Starts_with(U"-"))
		{
			arglist.push_back(utils::arguments::Argument(previous, replace));
			previous.clear();
		}
		else
		{
			arglist.push_back(utils::arguments::Argument(replace, {}));
			previous = replace;
		}
	}*/

	/*if (replace == original) 
	{
		previous = original;
	}
	else 
	{
		if (!previous.empty()) 
		{
			arglist.push_back(utils::arguments::Argument(previous, replace));
			previous.clear();
		}
		else 
		{
			arglist.push_back(utils::arguments::Argument(replace, {}));
		}
	}*/
}
