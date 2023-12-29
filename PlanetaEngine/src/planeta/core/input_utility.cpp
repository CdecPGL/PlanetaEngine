#include<unordered_map>

#include"input_utility.hpp"

namespace plnt::private_ {
	namespace {
		const std::unordered_map<keyboard::type, std::string> key_name_map = {
			{keyboard::a, "a"},
			{keyboard::b, "b"},
			{keyboard::c, "c"},
			{keyboard::d, "d"},
			{keyboard::e, "e"},
			{keyboard::f, "f"},
			{keyboard::g, "g"},
			{keyboard::h, "h"},
			{keyboard::i, "i"},
			{keyboard::j, "j"},
			{keyboard::k, "k"},
			{keyboard::l, "l"},
			{keyboard::m, "m"},
			{keyboard::n, "n"},
			{keyboard::o, "o"},
			{keyboard::p, "p"},
			{keyboard::q, "q"},
			{keyboard::r, "r"},
			{keyboard::s, "s"},
			{keyboard::t, "t"},
			{keyboard::u, "u"},
			{keyboard::v, "v"},
			{keyboard::w, "w"},
			{keyboard::x, "x"},
			{keyboard::y, "y"},
			{keyboard::z, "z"},
			{keyboard::space, "space"},
			{keyboard::l_shift, "l_shift"},
			{keyboard::l_control, "l_control"},
			{keyboard::enter, "enter"},
			{keyboard::num_0, "num_0"},
			{keyboard::num_1, "num_1"},
			{keyboard::num_2, "num_2"},
			{keyboard::num_3, "num_3"},
			{keyboard::num_4, "num_4"},
			{keyboard::num_5, "num_5"},
			{keyboard::num_6, "num_6"},
			{keyboard::num_7, "num_7"},
			{keyboard::num_8, "num_8"},
			{keyboard::num_9, "num_9"},
			{keyboard::f1, "f1"},
			{keyboard::f2, "f2"},
			{keyboard::f3, "f3"},
			{keyboard::f4, "f4"},
			{keyboard::f5, "f5"},
			{keyboard::f6, "f6"},
			{keyboard::f7, "f7"},
			{keyboard::f8, "f8"},
			{keyboard::f9, "f9"},
			{keyboard::f10, "f10"},
			{keyboard::f11, "f11"},
			{keyboard::f12, "f12"},
			{keyboard::esc, "escape"},
			{keyboard::r_shift, "r_shift"},
			{keyboard::r_control, "r_control"},
			{keyboard::up, "up"},
			{keyboard::down, "down"},
			{keyboard::left, "left"},
			{keyboard::right, "right"},
		};
		const std::unordered_map<pad::type, std::string> pad_name_map = {
			{pad::up, "up"},
			{pad::down, "down"},
			{pad::left, "left"},
			{pad::right, "right"},
			{pad::input_1, "input_1"},
			{pad::input_2, "input_2"},
			{pad::input_3, "input_3"},
			{pad::input_4, "input_4"},
			{pad::input_5, "input_5"},
			{pad::input_6, "input_6"},
			{pad::input_7, "input_7"},
			{pad::input_8, "input_8"},
			{pad::input_9, "input_9"},
			{pad::input_10, "input_10"},
			{pad::input_11, "input_11"},
			{pad::input_12, "input_12"},
			{pad::input_13, "input_13"},
			{pad::input_14, "input_14"},
			{pad::input_15, "input_15"},
			{pad::input_16, "input_16"},
			{pad::input_17, "input_17"},
			{pad::input_18, "input_18"},
			{pad::input_19, "input_19"},
			{pad::input_20, "input_20"},
			{pad::pov_up, "pov_up"},
			{pad::pov_down, "pov_down"},
			{pad::pov_left, "pov_left"},
			{pad::pov_right, "pov_right"},
		};
		const std::unordered_map<button::type, std::string> button_name_map = {
			{button::a, "a"},
			{button::b, "b"},
			{button::x, "x"},
			{button::y, "y"},
			{button::up, "up"},
			{button::down, "down"},
			{button::left, "left"},
			{button::right, "right"},
			{button::l, "l"},
			{button::r, "r"},
			{button::start, "start"},
		};
		std::unordered_map<std::string, keyboard::type> key_enum_map;
		std::unordered_map<std::string, pad::type> pad_enum_map;
		std::unordered_map<std::string, button::type> button_enum_map;
		bool is_knm_set = false;
		bool is_pnm_set = false;
		bool is_bnm_set = false;
		void set_key_name_map();
		void set_pad_name_map();
		void set_button_name_map();

		void set_key_name_map() {
			for (const auto &[e, n] : key_name_map) { key_enum_map.insert(std::make_pair(n, e)); }
			is_knm_set = true;
		}

		void set_pad_name_map() {
			for (const auto &[e, n] : pad_name_map) { pad_enum_map.insert(std::make_pair(n, e)); }
			is_pnm_set = true;
		}

		void set_button_name_map() {
			for (const auto &[e, n] : button_name_map) { button_enum_map.insert(std::make_pair(n, e)); }
			is_bnm_set = true;
		}
	}

	namespace utils {
		std::string convert_key_to_string(const keyboard::type k) {
			if (!is_knm_set) { set_key_name_map(); }
			const auto it = key_name_map.find(k);
			if (it == key_name_map.end()) { return "error"; }
			return it->second;
		}

		std::string convert_pad_to_string(const pad::type p) {
			if (!is_pnm_set) { set_pad_name_map(); }
			const auto it = pad_name_map.find(p);
			if (it == pad_name_map.end()) { return "error"; }
			return it->second;
		}

		std::string convert_button_to_string(const button::type b) {
			if (!is_bnm_set) { set_button_name_map(); }
			const auto it = button_name_map.find(b);
			if (it == button_name_map.end()) { return "error"; }
			return it->second;
		}

		keyboard::type convert_string_to_key(const std::string &ks) {
			if (!is_knm_set) { set_key_name_map(); }
			const auto it = key_enum_map.find(ks);
			if (it == key_enum_map.end()) { return keyboard::error; }
			return it->second;
		}

		pad::type convert_string_to_pad(const std::string &ps) {
			if (!is_pnm_set) { set_pad_name_map(); }
			const auto it = pad_enum_map.find(ps);
			if (it == pad_enum_map.end()) { return pad::error; }
			return it->second;
		}

		button::type convert_string_to_button(const std::string &bs) {
			if (!is_bnm_set) { set_button_name_map(); }
			const auto it = button_enum_map.find(bs);
			if (it == button_enum_map.end()) { return button::error; }
			return it->second;
		}
	}
}
