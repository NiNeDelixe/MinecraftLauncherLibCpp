#include "MinecraftLauncherLib/Utils/JsonUtils.h"

nlohmann::json MCLCPPLIB_NAMESPACE::utils::json::inheritJson(const nlohmann::json& original_data, const std::filesystem::path& minecraft_path)
{
	nlohmann::json inherit_version = original_data["inheritsFrom"];

	std::filesystem::path path_inh_json = 
		minecraft_path / "versions" / inherit_version.template get<std::string>() / (inherit_version.template get<std::string>() + ".json");

	nlohmann::json new_data;
	std::ifstream ifs;
	ifs.open(path_inh_json);
	ifs >> new_data;
	ifs.close();

	for (auto& var : original_data.items())
	{
		if (var.value().type() == nlohmann::json::value_t::array &&
			new_data[var.key()].type() == nlohmann::json::value_t::array)
		{
			for (auto& elem : var.value())
			{
				new_data[var.key()].push_back(elem);
			}
		}
		else if (var.value().type() == nlohmann::json::value_t::object &&
			new_data[var.key()].type() == nlohmann::json::value_t::object)
		{
			for (auto& variable : var.value().items())
			{
				if (variable.value().type() == nlohmann::json::value_t::array)
				{
					for (auto& elem : variable.value())
					{
						new_data[var.key()][variable.key()].push_back(elem);
					}
					//new_data[var.key()][variable.key()] += variable;
				}
			}
		}
		else
		{
			if (new_data.contains(var.key()))
			{
				new_data.erase(var.key());
			}
			new_data[var.key()] = var.value();
		}
	}

	return new_data;
}