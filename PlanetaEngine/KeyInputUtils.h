#pragma once

#include<string>

#include"KeyInputDefines.h"

namespace planeta_engine {
	namespace core {
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