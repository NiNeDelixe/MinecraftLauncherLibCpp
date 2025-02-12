#pragma once
#ifndef UTILS_DELEGATES_H_
#define UTILS_DELEGATES_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <functional>
#include <string>

#include <MinecraftLauncherLib/Utils/CallbackLevel.h>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace delegates
		{
			using runnable = std::function<void()>;
			template<class T> using supplier = std::function<T()>;
			template<class T> using consumer = std::function<void(T)>;

			// data sources
			using wstringsupplier = std::function<std::wstring()>;
			using u32stringsupplier = std::function<std::u32string()>;
			using doublesupplier = std::function<double()>;
			using boolsupplier = std::function<bool()>;

			using stringconsumer = std::function<void(const std::string&)>;
			using wstringconsumer = std::function<void(const std::wstring&)>;
			using doubleconsumer = std::function<void(double)>;
			using boolconsumer = std::function<void(bool)>;
			using int_array_consumer = std::function<void(const int[], size_t)>;
			using wstringchecker = std::function<bool(const std::wstring&)>;

			//current_progress, max_current_progress, all_tasks, text, callback_level
			//some params may be nullptr
			using callback_supplier = std::function<void(const size_t*, const size_t*, const size_t*, const char**, const utils::callback::CallbackLevel*)>;
		}
	}
}

#endif // !UTILS_DELEGATES_H_