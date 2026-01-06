#pragma once
#ifndef UTILS_STRINGUTILS_H_
#define UTILS_STRINGUTILS_H_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <string>
#include <vector>

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace string
		{
			template<typename STR>
			MCLLIB_API std::vector<STR> split(const STR& s, const char& seperator, const int& count = 0)
			{
				std::vector<STR> output;
				typename STR::value_type str_seperator = static_cast<typename STR::value_type>(seperator);
				typename STR::size_type prev_pos = 0, pos = 0;

				if (count > 0)
				{
					for (int i = 0; i < count; i++)
					{
						if ((pos = s.find(str_seperator, pos)) != STR::npos)
						{
							STR substring(s.substr(prev_pos, pos - prev_pos));

							output.push_back(substring);

							prev_pos = ++pos;
						}
					}
				}
				else if (count == 0)
				{
					while ((pos = s.find(str_seperator, pos)) != STR::npos)
					{
						STR substring(s.substr(prev_pos, pos - prev_pos));

						output.push_back(substring);

						prev_pos = ++pos;
					}
				}

				output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

				return output;
			}


			template<typename REPL, typename FROM, typename TO>
			MCLLIB_API void replace(REPL& replace, const FROM& from, const TO& to)
			{
				if (from.empty())
					return;

				REPL out{};
				auto it = replace.begin();
				auto from_it = from.begin();
				auto from_end = from.end();
				auto to_it = to.begin();

				while (it != replace.end())
				{
					if (*it == *from_it)
					{
						auto tmp_it = it;
						auto from_tmp_it = from_it;
						while (from_tmp_it != from_end && tmp_it != replace.end() && *tmp_it == *from_tmp_it)
						{
							++tmp_it;
							++from_tmp_it;
						}
						if (from_tmp_it == from_end)
						{
							out.insert(out.end(), to.begin(), to.end());
							it = tmp_it;
							from_it = from.begin();
							continue;
						}
					}
					out += static_cast<typename REPL::value_type>(*it);
					++it;
				}

				replace = std::move(out);
			}

			template<typename REPL>
			MCLLIB_API void replace(REPL& replace, const REPL& from, const REPL& to)
			{
				// size_t start_pos = 0;
				// size_t found_pos = 0;
				// bool replaced = false;
				
				// while ((found_pos = replace.find(from, start_pos)) != REPL::npos)
				// {
				//     replace.replace(found_pos, from.length(), to);
				//     start_pos = found_pos + to.length();
				//     replaced = true;
				// }
				replace<REPL, REPL, REPL>(replace_cont, from, to);
			}

			/*template<>
			void replace(std::wstring& replace, const std::wstring& from, const std::wstring& to)
			{
				std::wstring& replace_string = replace;
				std::wstring from_string = from;
				std::wstring to_string = to;

				size_t start_pos = 0;
				size_t found_pos = 0;
				bool replaced = false;

				while ((found_pos = replace_string.find(from_string, start_pos)) != std::string::npos)
				{
					replace_string.replace(found_pos, from_string.length(), to_string);
					start_pos = found_pos + to_string.length();
					replaced = true;
				}
			}*/

			template<typename STR>
			void ltrim(STR& s)
			{
				s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
					return !std::isspace(ch);
					}));
			}

			template<typename STR, typename SYMB>
			void ltrim(STR& s, const SYMB& symbol)
			{
				s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&symbol](SYMB ch) {
					return ch != symbol;
					}));
			}

			template<typename STR>
			void rtrim(STR& s)
			{
				s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
					return !std::isspace(ch);
					}).base(), s.end());
			}

			template<typename STR, typename SYMB>
			static inline void rtrim(STR& s, const SYMB& symbol)
			{
				s.erase(std::find_if(s.rbegin(), s.rend(), [&symbol](SYMB ch) {
					return ch != symbol;
					}).base(), s.end());
			}

			template<typename STR>
			void trim(STR& s)
			{
				rtrim(s);
				ltrim(s);
			}

			template<typename STR, typename SYMB>
			void trim(STR& s, const SYMB& symbol)
			{
				rtrim(s, symbol);
				ltrim(s, symbol);
			}

			template<typename STR>
			STR ltrim_copy(STR s)
			{
				ltrim(s);
				return s;
			}

			template<typename STR, typename SYMB>
			STR ltrim_copy(STR s, const SYMB& symbol)
			{
				ltrim(s, symbol);
				return s;
			}

			template<typename STR>
			STR rtrim_copy(STR s)
			{
				rtrim(s);
				return s;
			}

			template<typename STR, typename SYMB>
			STR rtrim_copy(STR s, const SYMB& symbol)
			{
				rtrim(s, symbol);
				return s;
			}

			template<typename STR>
			STR trim_copy(STR s)
			{
				trim(s);
				return s;
			}

			template<typename STR, typename SYMB>
			STR trim_copy(STR s, const SYMB& symbol)
			{
				trim(s, symbol);
				return s;
			}

		}
	}
}

#endif // !UTILS_STRINGUTILS_H_
