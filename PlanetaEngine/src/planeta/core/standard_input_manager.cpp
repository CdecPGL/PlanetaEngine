#include <cassert>

#include"DxLib.h"

#include "standard_input_manager.hpp"

namespace plnt {
	namespace {
		namespace my_pov_dir {
			constexpr unsigned char up = 1u << 0;
			constexpr unsigned char down = 1u << 1;
			constexpr unsigned char left = 1u << 2;
			constexpr unsigned char right = 1u << 3;
		}
	}

	namespace private_ {
		class standard_input_manager::impl {
			char dx_key_input_[256];
			char dx_key_input_pre_[256];
			int dx_pad_input_ = 0;
			int dx_pad_input_pre_ = 0;
			unsigned char dx_pov_input_ = 0; //ハットスイッチ(1bit:UP 2bit:DOWN 3bit:LEFT 4bit:RIGHT)
			unsigned char dx_pov_input_pre_ = 0;
			int dx_mouse_wheel_rotation_ = 0;
			int dx_mouse_button_input_ = 0;
			int dx_mouse_button_input_pre_ = 0;
			const static std::unordered_map<keyboard::type, int> my_key_dx_key_map;
			const static std::unordered_map<pad::type, int> my_pad_dx_pad_map;
			const static std::unordered_map<mouse_button::type, int> my_mouse_button_dx_mouse_button_map;
			const static std::unordered_map<pad::type, int> pov_map;

		public:
			std::unordered_map<button::type, std::vector<keyboard::type>> key_to_button_map;
			std::unordered_map<button::type, std::vector<pad::type>> pad_to_button_map;
			std::unordered_map<std::string, button::type> alias_button_map;
			impl();
			void set_default_key_button_map();
			void set_default_pad_button_map();
			void update_dx_key_input();
			[[nodiscard]] static bool is_pov(const pad::type p) { return pov_map.contains(p); }

			[[nodiscard]] bool state_pov(const pad::type p) const {
				return dx_pov_input_ & pov_map.at(p) ? true : false;
			}

			//ハットスイッチ処理(ハットスイッチでなかったらfalse)
			[[nodiscard]] bool push_pov(const pad::type p) const {
				return dx_pov_input_ & ~dx_pov_input_pre_ & pov_map.at(p) ? true : false;
			} //ハットスイッチ処理(ハットスイッチでなかったらfalse)
			[[nodiscard]] bool get_dx_key_state(const keyboard::type k) const {
				return dx_key_input_[my_key_dx_key_map.at(k)] ? true : false;
			}

			[[nodiscard]] bool get_dx_key_push(const keyboard::type k) const {
				const int dk = my_key_dx_key_map.at(k);
				return dx_key_input_[dk] ? (dx_key_input_pre_[dk] ? false : true) : false;
			}

			[[nodiscard]] bool get_dx_pad_state(const pad::type p) const {
				if (is_pov(p)) { return state_pov(p); }
				return dx_pad_input_ & my_pad_dx_pad_map.at(p) ? true : false;
			}

			[[nodiscard]] bool get_dx_pad_push(const pad::type p) const {
				if (is_pov(p)) { return push_pov(p); }
				return dx_pad_input_ & ~dx_pad_input_pre_ & my_pad_dx_pad_map.at(p) ? true : false;
			}

			[[nodiscard]] bool get_dx_mouse_button_state(const mouse_button::type mb) const {
				return dx_mouse_button_input_ & my_mouse_button_dx_mouse_button_map.at(mb) ? true : false;
			}

			[[nodiscard]] bool get_dx_mouse_button_push(const mouse_button::type mb) const {
				const int dk = my_mouse_button_dx_mouse_button_map.at(mb);
				return dx_mouse_button_input_ & dk ? (dx_mouse_button_input_pre_ & dk ? false : true) : false;
			}

			void assign_key_to_button(const keyboard::type k, button::type b) {
				if (k == 0) { return; }
				auto it = key_to_button_map.find(b);
				if (it == key_to_button_map.end()) {
					it = key_to_button_map.insert(std::make_pair(b, std::vector<keyboard::type>())).first;
				}
				std::vector<keyboard::type> &v = it->second;
				keyboard::type i = 0;
				for (keyboard::type p = 1; i < sizeof(keyboard::type) * 8; ++i, p <<= 1) {
					if (k & p) { v.push_back(p); }
				}
			}

			[[nodiscard]] int get_dx_mouse_wheel_rotation() const { return dx_mouse_wheel_rotation_; }

			void assign_pad_to_button(const pad::type p, button::type b) {
				if (p == 0) { return; }
				auto it = pad_to_button_map.find(b);
				if (it == pad_to_button_map.end()) {
					it = pad_to_button_map.insert(std::make_pair(b, std::vector<pad::type>())).first;
				}
				std::vector<pad::type> &v = it->second;
				pad::type i = 0;
				for (pad::type pp = 1; i < sizeof(pad::type) * 8; ++i, pp <<= 1) { if (p & pp) { v.push_back(pp); } }
			}
		};

		const std::unordered_map<keyboard::type, int> standard_input_manager::impl::my_key_dx_key_map = {
			{keyboard::a, KEY_INPUT_A},
			{keyboard::b, KEY_INPUT_B},
			{keyboard::c, KEY_INPUT_C},
			{keyboard::d, KEY_INPUT_D},
			{keyboard::e, KEY_INPUT_E},
			{keyboard::f, KEY_INPUT_F},
			{keyboard::g, KEY_INPUT_G},
			{keyboard::h, KEY_INPUT_H},
			{keyboard::i, KEY_INPUT_I},
			{keyboard::j, KEY_INPUT_J},
			{keyboard::k, KEY_INPUT_K},
			{keyboard::l, KEY_INPUT_L},
			{keyboard::m, KEY_INPUT_M},
			{keyboard::n, KEY_INPUT_N},
			{keyboard::o, KEY_INPUT_O},
			{keyboard::p, KEY_INPUT_P},
			{keyboard::q, KEY_INPUT_Q},
			{keyboard::r, KEY_INPUT_R},
			{keyboard::s, KEY_INPUT_S},
			{keyboard::t, KEY_INPUT_T},
			{keyboard::u, KEY_INPUT_U},
			{keyboard::v, KEY_INPUT_V},
			{keyboard::w, KEY_INPUT_W},
			{keyboard::x, KEY_INPUT_X},
			{keyboard::y, KEY_INPUT_Y},
			{keyboard::z, KEY_INPUT_Z},
			{keyboard::space, KEY_INPUT_SPACE},
			{keyboard::l_shift, KEY_INPUT_LSHIFT},
			{keyboard::enter, KEY_INPUT_RETURN},
			{keyboard::l_control, KEY_INPUT_LCONTROL},
			{keyboard::num_0, KEY_INPUT_0},
			{keyboard::num_1, KEY_INPUT_1},
			{keyboard::num_2, KEY_INPUT_2},
			{keyboard::num_3, KEY_INPUT_3},
			{keyboard::num_4, KEY_INPUT_4},
			{keyboard::num_5, KEY_INPUT_5},
			{keyboard::num_6, KEY_INPUT_6},
			{keyboard::num_7, KEY_INPUT_7},
			{keyboard::num_8, KEY_INPUT_8},
			{keyboard::num_9, KEY_INPUT_9},
			{keyboard::f1, KEY_INPUT_F1},
			{keyboard::f2, KEY_INPUT_F2},
			{keyboard::f3, KEY_INPUT_F3},
			{keyboard::f4, KEY_INPUT_F4},
			{keyboard::f5, KEY_INPUT_F5},
			{keyboard::f6, KEY_INPUT_F6},
			{keyboard::f7, KEY_INPUT_F7},
			{keyboard::f8, KEY_INPUT_F8},
			{keyboard::f9, KEY_INPUT_F9},
			{keyboard::f10, KEY_INPUT_F10},
			{keyboard::f11, KEY_INPUT_F11},
			{keyboard::f12, KEY_INPUT_F12},
			{keyboard::esc, KEY_INPUT_ESCAPE},
			{keyboard::r_shift, KEY_INPUT_RSHIFT},
			{keyboard::r_control, KEY_INPUT_RCONTROL},
			{keyboard::up, KEY_INPUT_UP},
			{keyboard::down, KEY_INPUT_DOWN},
			{keyboard::left, KEY_INPUT_LEFT},
			{keyboard::right, KEY_INPUT_RIGHT},
		};
		const std::unordered_map<pad::type, int> standard_input_manager::impl::my_pad_dx_pad_map = {
			{pad::up, PAD_INPUT_UP},
			{pad::down, PAD_INPUT_DOWN},
			{pad::left, PAD_INPUT_LEFT},
			{pad::right, PAD_INPUT_RIGHT},
			{pad::input_1, PAD_INPUT_1},
			{pad::input_2, PAD_INPUT_2},
			{pad::input_3, PAD_INPUT_3},
			{pad::input_4, PAD_INPUT_4},
			{pad::input_5, PAD_INPUT_5},
			{pad::input_6, PAD_INPUT_6},
			{pad::input_7, PAD_INPUT_7},
			{pad::input_8, PAD_INPUT_8},
			{pad::input_9, PAD_INPUT_9},
			{pad::input_10, PAD_INPUT_10},
			{pad::input_11, PAD_INPUT_11},
			{pad::input_12, PAD_INPUT_12},
			{pad::input_13, PAD_INPUT_13},
			{pad::input_14, PAD_INPUT_14},
			{pad::input_15, PAD_INPUT_15},
			{pad::input_16, PAD_INPUT_16},
			{pad::input_17, PAD_INPUT_17},
			{pad::input_18, PAD_INPUT_18},
			{pad::input_19, PAD_INPUT_19},
			{pad::input_20, PAD_INPUT_20},
		};
		const std::unordered_map<mouse_button::type, int>
		standard_input_manager::impl::my_mouse_button_dx_mouse_button_map =
		{
			{mouse_button::left,MOUSE_INPUT_LEFT},
			{mouse_button::right,MOUSE_INPUT_RIGHT},
			{mouse_button::middle,MOUSE_INPUT_MIDDLE},
			{mouse_button::input_4,MOUSE_INPUT_4},
			{mouse_button::input_5,MOUSE_INPUT_5},
			{mouse_button::input_6,MOUSE_INPUT_6},
			{mouse_button::input_7,MOUSE_INPUT_7},
			{mouse_button::input_8,MOUSE_INPUT_8},
		};
		const std::unordered_map<pad::type, int> standard_input_manager::impl::pov_map = {
			{pad::pov_up, my_pov_dir::up},
			{pad::pov_down, my_pov_dir::down},
			{pad::pov_left, my_pov_dir::left},
			{pad::pov_right, my_pov_dir::right},
		};

		standard_input_manager::impl::impl() {
			for (int i = 0; i < 256; ++i) {
				dx_key_input_pre_[i] = 0;
				dx_key_input_[i] = 0;
			}
		}

		void standard_input_manager::impl::update_dx_key_input() {
			//マウスホイール
			dx_mouse_wheel_rotation_ = GetMouseWheelRotVol();
			//マウスボタン
			dx_mouse_button_input_pre_ = dx_mouse_button_input_;
			dx_mouse_button_input_ = GetMouseInput();
			//キーボード
			for (int i = 0; i < 256; ++i) { dx_key_input_pre_[i] = dx_key_input_[i]; }
			GetHitKeyStateAll(dx_key_input_);
			//ジョイパッド
			if (GetJoypadNum() > 0) {
				dx_pad_input_pre_ = dx_pad_input_;
				dx_pad_input_ = GetJoypadInputState(DX_INPUT_PAD1);
				//pov
				dx_pov_input_pre_ = dx_pov_input_;
				DINPUT_JOYSTATE dj;
				GetJoypadDirectInputState(DX_INPUT_PAD1, &dj);
				/*const unsigned int& p0 = dj.POV[0];
				if (p0 == 0xffffffff){ dx_pov_input = 0; }
				else if (33750 <= p0 || p0 < 2250){ dx_pov_input = MY_POV_DIR::UP; }
				else if (2250 <= p0 && p0 < 6750){ dx_pov_input = MY_POV_DIR::UP | MY_POV_DIR::RIGHT; }
				else if (6750 <= p0 && p0 < 11250){ dx_pov_input = MY_POV_DIR::RIGHT; }
				else if (11250 <= p0 && p0 < 15750){ dx_pov_input = MY_POV_DIR::RIGHT | MY_POV_DIR::DOWN; }
				else if (15750 <= p0 && p0 < 20250){ dx_pov_input = MY_POV_DIR::DOWN; }
				else if (20250 <= p0 && p0 < 24750){ dx_pov_input = MY_POV_DIR::DOWN | MY_POV_DIR::LEFT; }
				else if (24750 <= p0 && p0 < 29250){ dx_pov_input = MY_POV_DIR::LEFT; }
				else if (29250 <= p0 && p0 < 33750){ dx_pov_input = MY_POV_DIR::LEFT | MY_POV_DIR::UP; }*/

				if (dj.POV[0] == 0xffffffff) { dx_pov_input_ = 0; } else {
					switch ((dj.POV[0] + 2250) % 36000 / 4500) {
						case 0:
							dx_pov_input_ = my_pov_dir::up;
							break;
						case 1:
							dx_pov_input_ = my_pov_dir::up | my_pov_dir::right;
							break;
						case 2:
							dx_pov_input_ = my_pov_dir::right;
							break;
						case 3:
							dx_pov_input_ = my_pov_dir::right | my_pov_dir::down;
							break;
						case 4:
							dx_pov_input_ = my_pov_dir::down;
							break;
						case 5:
							dx_pov_input_ = my_pov_dir::down | my_pov_dir::left;
							break;
						case 6:
							dx_pov_input_ = my_pov_dir::left;
							break;
						case 7:
							dx_pov_input_ = my_pov_dir::left | my_pov_dir::up;
							break;
						default:
							assert(false);
					}
				}
			}
		}

		void standard_input_manager::impl::set_default_key_button_map() {
			assign_key_to_button(keyboard::up, button::up);
			assign_key_to_button(keyboard::down, button::down);
			assign_key_to_button(keyboard::left, button::left);
			assign_key_to_button(keyboard::right, button::right);
			assign_key_to_button(keyboard::z, button::a);
			assign_key_to_button(keyboard::x, button::b);
			assign_key_to_button(keyboard::a, button::x);
			assign_key_to_button(keyboard::s, button::y);
			assign_key_to_button(keyboard::space, button::start);
			assign_key_to_button(keyboard::l_shift, button::l);
			assign_key_to_button(keyboard::l_control, button::r);
		}

		void standard_input_manager::impl::set_default_pad_button_map() {
			assign_pad_to_button(pad::up | pad::pov_up, button::up);
			assign_pad_to_button(pad::down | pad::pov_down, button::down);
			assign_pad_to_button(pad::left | pad::pov_left, button::left);
			assign_pad_to_button(pad::right | pad::pov_right, button::right);
			assign_pad_to_button(pad::input_3, button::a);
			assign_pad_to_button(pad::input_4, button::b);
			assign_pad_to_button(pad::input_1, button::x);
			assign_pad_to_button(pad::input_2, button::y);
			assign_pad_to_button(pad::input_12, button::start);
			assign_pad_to_button(pad::input_5, button::l);
			assign_pad_to_button(pad::input_6, button::r);
		}

		standard_input_manager::standard_input_manager() : impl_(std::make_unique<impl>()) {}

		bool standard_input_manager::initialize() { return true; }

		void standard_input_manager::reset_all_settings() {
			reset_key_pad_button_map();
			reset_button_alias();
		}

		void standard_input_manager::reset_key_pad_button_map() {
			impl_->key_to_button_map.clear();
			impl_->pad_to_button_map.clear();
		}

		void standard_input_manager::reset_button_alias() { impl_->alias_button_map.clear(); }

		void standard_input_manager::set_default_settings() {
			impl_->set_default_key_button_map();
			impl_->set_default_pad_button_map();
		}

		bool standard_input_manager::update() {
			impl_->update_dx_key_input();
			return true;
		}

		bool standard_input_manager::key_state(const keyboard::type k) const { return impl_->get_dx_key_state(k); }

		bool standard_input_manager::key_push(const keyboard::type k) const { return impl_->get_dx_key_push(k); }

		bool standard_input_manager::pad_state(const pad::type k) const { return impl_->get_dx_pad_state(k); }

		bool standard_input_manager::pad_push(const pad::type k) const { return impl_->get_dx_pad_push(k); }

		bool standard_input_manager::button_state(const button::type b) const {
			//キー
			if (const auto kit = impl_->key_to_button_map.find(b); kit != impl_->key_to_button_map.end()) {
				for (const keyboard::type k : kit->second) { if (key_state(k)) { return true; } }
			}
			//パッド
			if (const auto pit = impl_->pad_to_button_map.find(b); pit != impl_->pad_to_button_map.end()) {
				for (const pad::type k : pit->second) { if (pad_state(k)) { return true; } }
			}
			return false;
		}

		bool standard_input_manager::button_push(const button::type b) const {
			//キー
			if (const auto kit = impl_->key_to_button_map.find(b); kit != impl_->key_to_button_map.end()) {
				for (const keyboard::type k : kit->second) { if (key_push(k)) { return true; } }
			}
			//パッド
			if (const auto pit = impl_->pad_to_button_map.find(b); pit != impl_->pad_to_button_map.end()) {
				for (const pad::type k : pit->second) { if (pad_push(k)) { return true; } }
			}
			return false;
		}

		bool standard_input_manager::button_state(const std::string &a) const {
			const auto it = impl_->alias_button_map.find(a);
			if (it == impl_->alias_button_map.end()) { return false; }
			return button_state(it->second);
		}

		bool standard_input_manager::button_push(const std::string &a) const {
			const auto it = impl_->alias_button_map.find(a);
			if (it == impl_->alias_button_map.end()) { return false; }
			return button_push(it->second);
		}

		bool standard_input_manager::mouse_button_state(const mouse_button::type mb) const {
			return impl_->get_dx_mouse_button_state(mb);
		}

		bool standard_input_manager::mouse_button_push(const mouse_button::type mb) const {
			return impl_->get_dx_mouse_button_push(mb);
		}

		void standard_input_manager::assign_alias_to_button(const std::string &a, button::type b) {
			auto it = impl_->alias_button_map.find(a);
			if (it == impl_->alias_button_map.end()) {
				it = impl_->alias_button_map.insert(std::make_pair(a, b)).first;
			}
			it->second = b;
		}

		void standard_input_manager::assign_key_to_button(const keyboard::type k, const button::type b) {
			impl_->assign_key_to_button(k, b);
		}

		void standard_input_manager::assign_pad_to_button(const pad::type p, const button::type b) {
			impl_->assign_pad_to_button(p, b);
		}

		const std::unordered_map<button::type, std::vector<keyboard::type>> &
		standard_input_manager::get_assigned_key_to_button_map() const { return impl_->key_to_button_map; }

		const std::unordered_map<button::type, std::vector<pad::type>> &
		standard_input_manager::get_assigned_pad_to_button_map() const { return impl_->pad_to_button_map; }

		standard_input_manager::~standard_input_manager() = default;

		const vector_2di standard_input_manager::get_mouse_pointer_position() const {
			//			static int* leak = new int[2];
			//			static int* leak2 = new int;
			//			leak[3] = 0x666;
			//			leak[4] = 0x664;
			//			*(leak - 1) = 0x12345;
			////			assert(_CrtCheckMemory());
			int x, y;
			GetMousePoint(&x, &y);
			return vector_2di(x, y);
		}

		int standard_input_manager::get_mouse_wheel_rotation() const { return impl_->get_dx_mouse_wheel_rotation(); }

		void standard_input_manager::finalize() {}
	}
}

#undef TAG_CHECK_PROC
