#pragma once

#include<string>

#include"input_definitions.hpp"

namespace plnt::private_::utils {
	std::string convert_key_to_string(keyboard::type);
	std::string convert_pad_to_string(pad::type);
	std::string convert_button_to_string(button::type);
	keyboard::type convert_string_to_key(const std::string &);
	pad::type convert_string_to_pad(const std::string &);
	button::type convert_string_to_button(const std::string &);
}
