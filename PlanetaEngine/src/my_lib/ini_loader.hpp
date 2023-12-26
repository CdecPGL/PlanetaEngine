#pragma once

#include"ini_data.hpp"

class ini_loader {
public:
	// INIファイル読み込み
	int load_ini(const std::string &);
	// セクション取得(セクション名)
	std::unordered_map<std::string, std::string> operator[](const std::string &) const;
	// セクション取得(セクション名)
	[[nodiscard]] std::unordered_map<std::string, std::string> get_section(const std::string &) const;
	// セクションが存在するか確認
	[[nodiscard]] bool check_section_existence(const std::string &) const;
	// 値取得(セクション名,値名)(無かったらエラーメッセージか返る)
	[[nodiscard]] std::string get_value(const std::string &, const std::string &) const;

	[[nodiscard]] const ini_data &get_ini_data() const { return data_; }

private:
	ini_data data_;
};
