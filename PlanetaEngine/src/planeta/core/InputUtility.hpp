#pragma once

#include<string>

#include"planeta/core/InputDefinitions.hpp"

namespace plnt {
	namespace private_ {
		namespace utils {
			std::string ConvertKeyToString(Key::type);
			std::string ConvertPadToString(Pad::type);
			std::string ConvertButtonToString(Button::type);
			Key::type ConvertStringToKey(const std::string&);
			Pad::type ConvertStringToPad(const std::string&);
			Button::type ConvertStringToButton(const std::string&);
		}
	}
}