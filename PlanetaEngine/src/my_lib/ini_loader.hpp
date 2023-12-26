#pragma once

#include"ini_data.hpp"

class ini_loader {
public:
	// INI�t�@�C���ǂݍ���
	int load_ini(const std::string &);
	// �Z�N�V�����擾(�Z�N�V������)
	std::unordered_map<std::string, std::string> operator[](const std::string &) const;
	// �Z�N�V�����擾(�Z�N�V������)
	[[nodiscard]] std::unordered_map<std::string, std::string> get_section(const std::string &) const;
	// �Z�N�V���������݂��邩�m�F
	[[nodiscard]] bool check_section_existence(const std::string &) const;
	// �l�擾(�Z�N�V������,�l��)(����������G���[���b�Z�[�W���Ԃ�)
	[[nodiscard]] std::string get_value(const std::string &, const std::string &) const;

	[[nodiscard]] const ini_data &get_ini_data() const { return data_; }

private:
	ini_data data_;
};
