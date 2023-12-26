#pragma once
#include "Object.hpp"
#include <unordered_map>

namespace plnt {
	namespace util {
		class ParameterHolder final : public Object {
		public:
			/*フラグを設定(名前、値)*/
			bool SetFlag(const std::string &name, bool value) { _flag_map[name] = value; }
			/*フラグを取得(名前、デフォルト値(存在しなかった場合の値))*/
			bool GetFlag(const std::string &name, bool default_value) {
				try { return _flag_map.at(name); } catch (std::out_of_range &) { return default_value; }
			}

			/*整数を設定(名前、値)*/
			void SetInteger(const std::string &name, int value) { _integer_map[name] = value; }
			/*整数を取得(名前、デフォルト値(存在しなかった場合の値))*/
			int GetInteger(const std::string &name, int default_value) {
				try { return _integer_map.at(name); } catch (std::out_of_range &) { return default_value; }
			}

			/*実数を設定(名前、値)*/
			void SetNumber(const std::string &name, double value) { _number_map[name] = value; }
			/*実数を取得(名前、デフォルト値(存在しなかった場合の値))*/
			double GetNumbder(const std::string &name, double default_value) {
				try { return _number_map.at(name); } catch (std::out_of_range &) { return default_value; }
			}

			/*文字列を設定(名前、値)*/
			void SetString(const std::string &name, const std::string &value) { _string_map[name] = value; }
			/*文字列を取得(名前、デフォルト値(存在しなかった場合の値))*/
			const std::string GetString(const std::string &name, const std::string &default_value) {
				try { return _string_map.at(name); } catch (std::out_of_range &) { return default_value; }
			}

			/*すべてのパラメータを削除*/
			void ClearAllParameters() {
				_flag_map.clear();
				_integer_map.clear();
				_number_map.clear();
				_string_map.clear();
			}

		private:
			std::unordered_map<std::string, bool> _flag_map;
			std::unordered_map<std::string, int> _integer_map;
			std::unordered_map<std::string, double> _number_map;
			std::unordered_map<std::string, std::string> _string_map;
		};
	}
}
