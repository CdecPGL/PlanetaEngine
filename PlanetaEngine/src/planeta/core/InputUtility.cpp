#include"InputUtility.hpp"

#include<unordered_map>

namespace plnt {
	namespace private_ {
		namespace {
			const std::unordered_map<Key::type, std::string> _key_name_map = {
				{Key::A, "A"},
				{Key::B, "B"},
				{Key::C, "C"},
				{Key::D, "D"},
				{Key::E, "E"},
				{Key::F, "F"},
				{Key::G, "G"},
				{Key::H, "H"},
				{Key::I, "I"},
				{Key::J, "J"},
				{Key::K, "K"},
				{Key::L, "L"},
				{Key::M, "M"},
				{Key::N, "N"},
				{Key::O, "O"},
				{Key::P, "P"},
				{Key::Q, "Q"},
				{Key::R, "R"},
				{Key::S, "S"},
				{Key::T, "T"},
				{Key::U, "U"},
				{Key::V, "V"},
				{Key::W, "W"},
				{Key::X, "X"},
				{Key::Y, "Y"},
				{Key::Z, "Z"},
				{Key::SPACE, "Space"},
				{Key::LSHIFT, "LeftShift"},
				{Key::LCONTROL, "LeftControl"},
				{Key::ENTER, "Enter"},
				{Key::NUM_0, "Num0"},
				{Key::NUM_1, "Num1"},
				{Key::NUM_2, "Num2"},
				{Key::NUM_3, "Num3"},
				{Key::NUM_4, "Num4"},
				{Key::NUM_5, "Num5"},
				{Key::NUM_6, "Num6"},
				{Key::NUM_7, "Num7"},
				{Key::NUM_8, "Num8"},
				{Key::NUM_9, "Num9"},
				{Key::F1, "F1"},
				{Key::F2, "F2"},
				{Key::F3, "F3"},
				{Key::F4, "F4"},
				{Key::F5, "F5"},
				{Key::F6, "F6"},
				{Key::F7, "F7"},
				{Key::F8, "F8"},
				{Key::F9, "F9"},
				{Key::F10, "F10"},
				{Key::F11, "F11"},
				{Key::F12, "F12"},
				{Key::ESC, "Escape"},
				{Key::RSHIFT, "RightShift"},
				{Key::RCONTROL, "RightControl"},
				{Key::UP, "Up"},
				{Key::DOWN, "Down"},
				{Key::LEFT, "Left"},
				{Key::RIGHT, "Right"},
			};
			const std::unordered_map<Pad::type, std::string> _pad_name_map = {
				{Pad::UP, "Up"},
				{Pad::DOWN, "Down"},
				{Pad::LEFT, "Left"},
				{Pad::RIGHT, "Right"},
				{Pad::INPUT_1, "Input1"},
				{Pad::INPUT_2, "Input2"},
				{Pad::INPUT_3, "Input3"},
				{Pad::INPUT_4, "Input4"},
				{Pad::INPUT_5, "Input5"},
				{Pad::INPUT_6, "Input6"},
				{Pad::INPUT_7, "Input7"},
				{Pad::INPUT_8, "Input8"},
				{Pad::INPUT_9, "Input9"},
				{Pad::INPUT_10, "Input10"},
				{Pad::INPUT_11, "Input11"},
				{Pad::INPUT_12, "Input12"},
				{Pad::INPUT_13, "Input13"},
				{Pad::INPUT_14, "Input14"},
				{Pad::INPUT_15, "Input15"},
				{Pad::INPUT_16, "Input16"},
				{Pad::INPUT_17, "Input17"},
				{Pad::INPUT_18, "Input18"},
				{Pad::INPUT_19, "Input19"},
				{Pad::INPUT_20, "Input20"},
				{Pad::POV_UP, "PovUp"},
				{Pad::POV_DOWN, "PovDown"},
				{Pad::POV_LEFT, "PovLeft"},
				{Pad::POV_RIGHT, "PovRight"},
			};
			const std::unordered_map<Button::type, std::string> _button_name_map = {
				{Button::A, "A"},
				{Button::B, "B"},
				{Button::X, "X"},
				{Button::Y, "Y"},
				{Button::UP, "Up"},
				{Button::DOWN, "Down"},
				{Button::LEFT, "Left"},
				{Button::RIGHT, "Right"},
				{Button::L, "L"},
				{Button::R, "R"},
				{Button::START, "Start"},
			};
			std::unordered_map<std::string, Key::type> _key_name_mapr;
			std::unordered_map<std::string, Pad::type> _pad_name_mapr;
			std::unordered_map<std::string, Button::type> _button_name_mapr;
			bool _knm_setted = false;
			bool _pnm_setted = false;
			bool _bnm_setted = false;
			void setKeyNameMap();
			void setPadNameMap();
			void setButtonNameMap();

			void setKeyNameMap() {
				for (const auto &p : _key_name_map) { _key_name_mapr.insert(std::make_pair(p.second, p.first)); }
				_knm_setted = true;
			}

			void setPadNameMap() {
				for (const auto &p : _pad_name_map) { _pad_name_mapr.insert(std::make_pair(p.second, p.first)); }
				_pnm_setted = true;
			}

			void setButtonNameMap() {
				for (const auto &p : _button_name_map) { _button_name_mapr.insert(std::make_pair(p.second, p.first)); }
				_bnm_setted = true;
			}
		}

		namespace utils {
			std::string ConvertKeyToString(Key::type k) {
				if (!_knm_setted) { setKeyNameMap(); }
				auto it = _key_name_map.find(k);
				if (it == _key_name_map.end()) { return "Error"; }
				return it->second;
			}

			std::string ConvertPadToString(Pad::type p) {
				if (!_pnm_setted) { setPadNameMap(); }
				auto it = _pad_name_map.find(p);
				if (it == _pad_name_map.end()) { return "Error"; }
				return it->second;
			}

			std::string ConvertButtonToString(Button::type b) {
				if (!_bnm_setted) { setButtonNameMap(); }
				auto it = _button_name_map.find(b);
				if (it == _button_name_map.end()) { return "Error"; }
				return it->second;
			}

			Key::type ConvertStringToKey(const std::string &ks) {
				if (!_knm_setted) { setKeyNameMap(); }
				auto it = _key_name_mapr.find(ks);
				if (it == _key_name_mapr.end()) { return Key::Error; }
				return it->second;
			}

			Pad::type ConvertStringToPad(const std::string &ps) {
				if (!_pnm_setted) { setPadNameMap(); }
				auto it = _pad_name_mapr.find(ps);
				if (it == _pad_name_mapr.end()) { return Pad::Error; }
				return it->second;
			}

			Button::type ConvertStringToButton(const std::string &bs) {
				if (!_bnm_setted) { setButtonNameMap(); }
				auto it = _button_name_mapr.find(bs);
				if (it == _button_name_mapr.end()) { return Button::Error; }
				return it->second;
			}
		}
	}
}
