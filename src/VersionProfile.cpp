#include "MinecraftLauncherLib/VersionProfile.h"

MCLCPPLIB_NAMESPACE::VersionProfile::VersionProfile(const std::string& version, const options::RuntimeOptions& options)
	: instance_path(options.minecraft_directory.get()), version(std::filesystem::path(version).u32string()) //FIXME: temperory
{
	fromJson(instance_path / "versions" / version / (version + ".json"));
}

void MCLCPPLIB_NAMESPACE::VersionProfile::fromJson(const std::filesystem::path& json_path, const options::RuntimeOptions& options)
{
	std::ifstream ifstr(json_path);
	nlohmann::json version_json = nlohmann::json::parse(ifstr);
	ifstr.close();

	if (version_json.contains("inheritsFrom"))
	{
		version_json = VersionProfile::inheritJson(version_json, this->instance_path);
	}

	if (this->natives_path.empty())
	{
		this->natives_path = this->instance_path / "versions" / version_json["id"].template get<std::string>() / "natives";
	}

	this->libraries_paths = libraries::Libraries::fromJson(version_json, this->instance_path, options.libraries_directory.get()).toPaths();

	this->java_mainclass = version_json["mainClass"].template get<std::filesystem::path>().u32string();
}

nlohmann::json MCLCPPLIB_NAMESPACE::VersionProfile::inheritJson(const nlohmann::json& original_data, const std::filesystem::path& path)
{
	/*
	Implement the inheritsFrom function
	See https://github.com/tomsik68/mclauncher-api/wiki/Version-Inheritance-&-forge
	*/

	nlohmann::json inherit_version = original_data["inheritsFrom"];

	std::filesystem::path path_inh_json = path / "versions" / inherit_version.template get<std::string>() / (inherit_version.template get<std::string>() + ".json");

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
			//qDebug() << new_data[var.key()];
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
