#include "MinecraftLauncherLib/Utils/Setting.h"

std::string MCLCPPLIB_NAMESPACE::utils::settings::NumberSetting::toString() const {
	switch (getFormat()) {
	case setting_format::simple:
		return std::to_string(value);
	case setting_format::percent:
		return std::to_string(static_cast<int64_t>(round(value * 100))) + "%";
	default:
		return "invalid format";
	}
}

std::string MCLCPPLIB_NAMESPACE::utils::settings::IntegerSetting::toString() const {
	switch (getFormat()) {
	case setting_format::simple:
		return std::to_string(value);
	case setting_format::percent:
		return std::to_string(value) + "%";
	default:
		return "invalid format";
	}
}

std::string MCLCPPLIB_NAMESPACE::utils::settings::FlagSetting::toString() const {
	switch (getFormat()) {
	case setting_format::simple:
		return value ? "true" : "false";
	default:
		return "invalid format";
	}
}

std::string MCLCPPLIB_NAMESPACE::utils::settings::StringSetting::toString() const {
	return value;
}

std::string MCLCPPLIB_NAMESPACE::utils::settings::PathSetting::toString() const
{
	return value.string();
}
