#pragma once

#include <unordered_map>

#include "object.hpp"

namespace plnt::util {
	class parameter_holder final : public object {
	public:
		/*フラグを設定(名前、値)*/
		void set_flag(const std::string &name, const bool value) { flag_map_[name] = value; }
		/*フラグを取得(名前、デフォルト値(存在しなかった場合の値))*/
		[[nodiscard]] bool get_flag(const std::string &name, const bool default_value) const {
			try { return flag_map_.at(name); } catch (std::out_of_range &) { return default_value; }
		}

		/*整数を設定(名前、値)*/
		void set_integer(const std::string &name, const int value) { integer_map_[name] = value; }
		/*整数を取得(名前、デフォルト値(存在しなかった場合の値))*/
		[[nodiscard]] int get_integer(const std::string &name, const int default_value) const {
			try { return integer_map_.at(name); } catch (std::out_of_range &) { return default_value; }
		}

		/*実数を設定(名前、値)*/
		void set_number(const std::string &name, const double value) { number_map_[name] = value; }
		/*実数を取得(名前、デフォルト値(存在しなかった場合の値))*/
		[[nodiscard]] double get_number(const std::string &name, const double default_value) const {
			try { return number_map_.at(name); } catch (std::out_of_range &) { return default_value; }
		}

		/*文字列を設定(名前、値)*/
		void set_string(const std::string &name, const std::string &value) { string_map_[name] = value; }
		/*文字列を取得(名前、デフォルト値(存在しなかった場合の値))*/
		[[nodiscard]] std::string get_string(const std::string &name, const std::string &default_value) {
			try { return string_map_.at(name); } catch (std::out_of_range &) { return default_value; }
		}

		/*すべてのパラメータを削除*/
		void clear_all_parameters() {
			flag_map_.clear();
			integer_map_.clear();
			number_map_.clear();
			string_map_.clear();
		}

	private:
		std::unordered_map<std::string, bool> flag_map_;
		std::unordered_map<std::string, int> integer_map_;
		std::unordered_map<std::string, double> number_map_;
		std::unordered_map<std::string, std::string> string_map_;
	};
}
