#include "StandardInputManager.hpp"
#include <cassert>
#include<algorithm>
#include"DxLib.h"

namespace plnt {
	namespace {
		namespace MY_POV_DIR {
			const unsigned char UP = 1u << 0;
			const unsigned char DOWN = 1u << 1;
			const unsigned char LEFT = 1u << 2;
			const unsigned char RIGHT = 1u << 3;
		}
	}

	namespace private_ {
		class StandardInputManager::Impl {
		private:
			char dx_key_input[256];
			char dx_key_input_pre[256];
			int dx_pad_input = 0;
			int dx_pad_input_pre = 0;
			unsigned char dx_pov_input = 0; //ハットスイッチ(1bit:UP 2bit:DOWN 3bit:LEFT 4bit:RIGHT)
			unsigned char dx_pov_input_pre = 0;
			int dx_mouse_wheel_rotation = 0;
			int dx_mouse_button_input = 0;
			int dx_mouse_button_input_pre = 0;
			const static std::unordered_map<keyboard::type, int> _mykey_dxkey_map;
			const static std::unordered_map<pad::type, int> _mypad_dxpad_map;
			const static std::unordered_map<mouse_button::type, int> _mymousebutton_dxmousebutton_map;
			const static std::unordered_map<pad::type, int> _pov_map;

		public:
			std::unordered_map<button::type, std::vector<keyboard::type>> _key_to_button_map;
			std::unordered_map<button::type, std::vector<pad::type>> _pad_to_button_map;
			std::unordered_map<std::string, button::type> _alias_button_map;
			Impl();
			~Impl() = default;
			void SetDefaultKeyButtonMap();
			void SetDefaultPadButtonMap();
			void UpdateDxKeyInput();
			bool isPov(pad::type p) const { return _pov_map.find(p) == _pov_map.end() ? false : true; }
			bool statePOV(pad::type p) const { return (dx_pov_input & _pov_map.at(p)) ? true : false; }
			//ハットスイッチ処理(ハットスイッチでなかったらfalse)
			bool pushPOV(pad::type p) const {
				return (dx_pov_input & ~dx_pov_input_pre & _pov_map.at(p)) ? true : false;
			} //ハットスイッチ処理(ハットスイッチでなかったらfalse)
			bool GetDxKeyState(keyboard::type k) const { return dx_key_input[_mykey_dxkey_map.at(k)] ? true : false; }

			bool GetDxKeyPush(keyboard::type k) const {
				int dk = _mykey_dxkey_map.at(k);
				return dx_key_input[dk] ? (dx_key_input_pre[dk] ? false : true) : false;
			}

			bool GetDxPadState(pad::type p) const {
				if (isPov(p)) { return statePOV(p); }
				return (dx_pad_input & _mypad_dxpad_map.at(p)) ? true : false;
			}

			bool GetDxPadPush(pad::type p) const {
				if (isPov(p)) { return pushPOV(p); }
				return (dx_pad_input & ~dx_pad_input_pre & _mypad_dxpad_map.at(p)) ? true : false;
			}

			bool GetDxMouseButtonState(mouse_button::type mb) const {
				return dx_mouse_button_input & _mymousebutton_dxmousebutton_map.at(mb) ? true : false;
			}

			bool GetDxMouseButtonPush(mouse_button::type mb) const {
				int dk = _mymousebutton_dxmousebutton_map.at(mb);
				return (dx_mouse_button_input & dk) ? ((dx_mouse_button_input_pre & dk) ? false : true) : false;
			}

			void AssignKeyToButton(keyboard::type k, button::type b) {
				if (k == 0) { return; }
				auto it = _key_to_button_map.find(b);
				if (it == _key_to_button_map.end()) {
					it = (_key_to_button_map.insert(std::make_pair(b, std::vector<keyboard::type>()))).first;
				}
				std::vector<keyboard::type> &v = it->second;
				int i = 0;
				for (keyboard::type p = 1; i < sizeof(keyboard::type) * 8; ++i, p <<= 1) { if (k & p) { v.push_back(p); } }
			}

			int GetDxMouseWheelRotation() const { return dx_mouse_wheel_rotation; }

			void AssignPadToButton(pad::type p, button::type b) {
				if (p == 0) { return; }
				auto it = _pad_to_button_map.find(b);
				if (it == _pad_to_button_map.end()) {
					it = (_pad_to_button_map.insert(std::make_pair(b, std::vector<pad::type>()))).first;
				}
				std::vector<pad::type> &v = it->second;
				int i = 0;
				for (pad::type pp = 1; i < sizeof(pad::type) * 8; ++i, pp <<= 1) { if (p & pp) { v.push_back(pp); } }
			}
		};

		const std::unordered_map<keyboard::type, int> StandardInputManager::Impl::_mykey_dxkey_map = {
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
		const std::unordered_map<pad::type, int> StandardInputManager::Impl::_mypad_dxpad_map = {
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
		const std::unordered_map<mouse_button::type, int> StandardInputManager::Impl::_mymousebutton_dxmousebutton_map =
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
		const std::unordered_map<pad::type, int> StandardInputManager::Impl::_pov_map = {
			{pad::pov_up, MY_POV_DIR::UP},
			{pad::pov_down, MY_POV_DIR::DOWN},
			{pad::pov_left, MY_POV_DIR::LEFT},
			{pad::pov_right, MY_POV_DIR::RIGHT},
		};

		StandardInputManager::Impl::Impl() : dx_pad_input(0), dx_pad_input_pre(0), dx_pov_input(0),
		                                     dx_pov_input_pre(0) {
			for (int i = 0; i < 256; ++i) {
				dx_key_input_pre[i] = 0;
				dx_key_input[i] = 0;
			}
		}

		void StandardInputManager::Impl::UpdateDxKeyInput() {
			//マウスホイール
			dx_mouse_wheel_rotation = GetMouseWheelRotVol();
			//マウスボタン
			dx_mouse_button_input_pre = dx_mouse_button_input;
			dx_mouse_button_input = GetMouseInput();
			//キーボード
			for (int i = 0; i < 256; ++i) { dx_key_input_pre[i] = dx_key_input[i]; }
			GetHitKeyStateAll(dx_key_input);
			//ジョイパッド
			if (GetJoypadNum() > 0) {
				dx_pad_input_pre = dx_pad_input;
				dx_pad_input = GetJoypadInputState(DX_INPUT_PAD1);
				//pov
				dx_pov_input_pre = dx_pov_input;
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

				if (dj.POV[0] == 0xffffffff) { dx_pov_input = 0; } else {
					int dn = (dj.POV[0] + 2250) % 36000 / 4500;
					switch (dn) {
						case 0:
							dx_pov_input = MY_POV_DIR::UP;
							break;
						case 1:
							dx_pov_input = MY_POV_DIR::UP | MY_POV_DIR::RIGHT;
							break;
						case 2:
							dx_pov_input = MY_POV_DIR::RIGHT;
							break;
						case 3:
							dx_pov_input = MY_POV_DIR::RIGHT | MY_POV_DIR::DOWN;
							break;
						case 4:
							dx_pov_input = MY_POV_DIR::DOWN;
							break;
						case 5:
							dx_pov_input = MY_POV_DIR::DOWN | MY_POV_DIR::LEFT;
							break;
						case 6:
							dx_pov_input = MY_POV_DIR::LEFT;
							break;
						case 7:
							dx_pov_input = MY_POV_DIR::LEFT | MY_POV_DIR::UP;
							break;
					}
				}
			}
		}

		void StandardInputManager::Impl::SetDefaultKeyButtonMap() {
			AssignKeyToButton(keyboard::up, button::up);
			AssignKeyToButton(keyboard::down, button::down);
			AssignKeyToButton(keyboard::left, button::left);
			AssignKeyToButton(keyboard::right, button::right);
			AssignKeyToButton(keyboard::z, button::a);
			AssignKeyToButton(keyboard::x, button::b);
			AssignKeyToButton(keyboard::a, button::x);
			AssignKeyToButton(keyboard::s, button::y);
			AssignKeyToButton(keyboard::space, button::start);
			AssignKeyToButton(keyboard::l_shift, button::l);
			AssignKeyToButton(keyboard::l_control, button::r);
		}

		void StandardInputManager::Impl::SetDefaultPadButtonMap() {
			AssignPadToButton(pad::up | pad::pov_up, button::up);
			AssignPadToButton(pad::down | pad::pov_down, button::down);
			AssignPadToButton(pad::left | pad::pov_left, button::left);
			AssignPadToButton(pad::right | pad::pov_right, button::right);
			AssignPadToButton(pad::input_3, button::a);
			AssignPadToButton(pad::input_4, button::b);
			AssignPadToButton(pad::input_1, button::x);
			AssignPadToButton(pad::input_2, button::y);
			AssignPadToButton(pad::input_12, button::start);
			AssignPadToButton(pad::input_5, button::l);
			AssignPadToButton(pad::input_6, button::r);
		}

		StandardInputManager::StandardInputManager() : _impl(std::make_unique<Impl>()) { }

		bool StandardInputManager::initialize() { return true; }

		void StandardInputManager::reset_all_settings() {
			reset_key_pad_button_map();
			reset_button_alias();
		}

		void StandardInputManager::reset_key_pad_button_map() {
			_impl->_key_to_button_map.clear();
			_impl->_pad_to_button_map.clear();
		}

		void StandardInputManager::reset_button_alias() { _impl->_alias_button_map.clear(); }

		void StandardInputManager::set_default_settings() {
			_impl->SetDefaultKeyButtonMap();
			_impl->SetDefaultPadButtonMap();
		}

		bool StandardInputManager::update() {
			_impl->UpdateDxKeyInput();
			return true;
		}

		bool StandardInputManager::key_state(keyboard::type k) const { return _impl->GetDxKeyState(k); }

		bool StandardInputManager::key_push(keyboard::type k) const { return _impl->GetDxKeyPush(k); }

		bool StandardInputManager::pad_state(pad::type k) const { return _impl->GetDxPadState(k); }

		bool StandardInputManager::pad_push(pad::type k) const { return _impl->GetDxPadPush(k); }

		bool StandardInputManager::button_state(button::type b) const {
			//キー
			auto kit = _impl->_key_to_button_map.find(b);
			if (kit != _impl->_key_to_button_map.end()) {
				for (keyboard::type k : kit->second) { if (key_state(k)) { return true; } }
			}
			//パッド
			auto pit = _impl->_pad_to_button_map.find(b);
			if (pit != _impl->_pad_to_button_map.end()) {
				for (pad::type k : pit->second) { if (pad_state(k)) { return true; } }
			}
			return false;
		}

		bool StandardInputManager::button_push(button::type b) const {
			//キー
			auto kit = _impl->_key_to_button_map.find(b);
			if (kit != _impl->_key_to_button_map.end()) {
				for (keyboard::type k : kit->second) { if (key_push(k)) { return true; } }
			}
			//パッド
			auto pit = _impl->_pad_to_button_map.find(b);
			if (pit != _impl->_pad_to_button_map.end()) {
				for (pad::type k : pit->second) { if (pad_push(k)) { return true; } }
			}
			return false;
		}

		bool StandardInputManager::button_state(const std::string &a) const {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) { return false; }
			return button_state(it->second);
		}

		bool StandardInputManager::button_push(const std::string &a) const {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) { return false; }
			return button_push(it->second);
		}

		bool StandardInputManager::mouse_button_state(mouse_button::type mb) const {
			return _impl->GetDxMouseButtonState(mb);
		}

		bool StandardInputManager::mouse_button_push(mouse_button::type mb) const {
			return _impl->GetDxMouseButtonPush(mb);
		}

		void StandardInputManager::assign_alias_to_button(const std::string &a, button::type b) {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) {
				it = (_impl->_alias_button_map.insert(std::make_pair(a, b))).first;
			}
			it->second = b;
		}

		void StandardInputManager::assign_key_to_button(keyboard::type k, button::type b) { _impl->AssignKeyToButton(k, b); }

		void StandardInputManager::assign_pad_to_button(pad::type p, button::type b) { _impl->AssignPadToButton(p, b); }

		const std::unordered_map<button::type, std::vector<keyboard::type>> &
		StandardInputManager::get_assigned_key_to_button_map() const { return _impl->_key_to_button_map; }

		const std::unordered_map<button::type, std::vector<pad::type>> &
		StandardInputManager::get_assigned_pad_to_button_map() const { return _impl->_pad_to_button_map; }

		StandardInputManager::~StandardInputManager() { }

		const Vector2Di StandardInputManager::get_mouse_pointer_position() const {
			//			static int* leak = new int[2];
			//			static int* leak2 = new int;
			//			leak[3] = 0x666;
			//			leak[4] = 0x664;
			//			*(leak - 1) = 0x12345;
			////			assert(_CrtCheckMemory());
			int x, y;
			GetMousePoint(&x, &y);
			return Vector2Di(x, y);
		}

		int StandardInputManager::get_mouse_wheel_rotation() const { return _impl->GetDxMouseWheelRotation(); }

		void StandardInputManager::finalize() { }
	}
}

#undef TAG_CHECK_PROC
