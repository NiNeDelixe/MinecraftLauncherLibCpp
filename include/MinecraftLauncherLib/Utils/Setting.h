#pragma once
#ifndef MINECRAFTLAUNCHERLIB_SETTING_H_
#define MINECRAFTLAUNCHERLIB_SETTING_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <limits>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <functional>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace settings
		{
			enum class setting_format
			{
				simple, percent
			};

			class Setting
			{
			protected:
				setting_format format;
			public:
				Setting(setting_format format) : format(format) {
				}

				virtual ~Setting() {}

				virtual void resetToDefault() = 0;

				virtual setting_format getFormat() const {
					return format;
				}

				virtual std::string toString() const = 0;
			};

			template<class T>
			class ObservableSetting : public Setting
			{
				int nextid = 1;
				std::unordered_map<int, std::function<void(T)>> observers;
			protected:
				T initial;
				T value;
			public:
				ObservableSetting(T value, setting_format format)
					: Setting(format), initial(value), value(value) {}

				std::shared_ptr<int> observe(std::function<void(T)> callback, bool callOnStart = false) {
					const int id = nextid++;
					observers.emplace(id, callback);
					if (callOnStart) {
						callback(value);
					}
					return std::shared_ptr<int>(new int(id), [this](int* id) {
						observers.erase(*id);
						delete id;
						});
				}

				const T& get() const {
					return value;
				}

				T& operator*() {
					return value;
				}

				void notify(T value) {
					for (auto& entry : observers) {
						entry.second(value);
					}
				}

				void set(T value) {
					if (value == this->value) {
						return;
					}
					this->value = value;
					notify(value);
				}

				virtual void resetToDefault() override {
					set(initial);
				}
			};

			class NumberSetting : public ObservableSetting<double>
			{
			protected:
				double min;
				double max;
			public:
				NumberSetting(
					double value,
					double min = std::numeric_limits<double>::min(),
					double max = std::numeric_limits<double>::max(),
					setting_format format = setting_format::simple
				) : ObservableSetting(value, format),
					min(min),
					max(max)
				{}

				double& operator*() {
					return value;
				}

				double get() const {
					return value;
				}

				double getMin() const {
					return min;
				}

				double getMax() const {
					return max;
				}

				double getT() const {
					return (value - min) / (max - min);
				}

				virtual std::string toString() const override;

				static inline NumberSetting createPercent(double def) {
					return NumberSetting(def, 0.0, 1.0, setting_format::percent);
				}
			};

			class IntegerSetting : public ObservableSetting<int64_t>
			{
			protected:
				int64_t min;
				int64_t max;
			public:
				IntegerSetting(
					int64_t value,
					int64_t min = std::numeric_limits<int64_t>::min(),
					int64_t max = std::numeric_limits<int64_t>::max(),
					setting_format format = setting_format::simple
				) : ObservableSetting(value, format),
					min(min),
					max(max)
				{}

				int64_t getMin() const {
					return min;
				}

				int64_t getMax() const {
					return max;
				}

				int64_t getT() const {
					return (value - min) / (max - min);
				}

				virtual std::string toString() const override;
			};

			class FlagSetting : public ObservableSetting<bool> {
			public:
				FlagSetting(
					bool value,
					setting_format format = setting_format::simple
				) : ObservableSetting(value, format) {}

				void toggle() {
					set(!get());
				}

				virtual std::string toString() const override;
			};

			class StringSetting : public ObservableSetting<std::string> {
			public:
				StringSetting(
					std::string value,
					setting_format format = setting_format::simple
				) : ObservableSetting(value, format) {}

				virtual std::string toString() const override;
			};

			class PathSetting : public ObservableSetting<std::filesystem::path>
			{
			public:
				PathSetting(const std::filesystem::path& path, setting_format format = setting_format::simple) 
					: ObservableSetting(path, format) {}
				~PathSetting() = default;

				virtual std::string toString() const override;
			};

		}
	}
}

#endif MINECRAFTLAUNCHERLIB_SETTING_H_