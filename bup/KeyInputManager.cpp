#include "KeyInputManager.h"
#include <cassert>
#include<algorithm>
#include"DxLib.h"

#define TAG_CHECK_PROC(t) if ((t) != "\0"){\
	auto tagit = _impl->_tag_invalid_flag.find(t);\
	if (tagit != _impl->_tag_invalid_flag.end() && tagit->second){ return false; }\
}

namespace planeta {
	namespace core {

		namespace {
			namespace MY_POV_DIR {
				const unsigned char UP = 1u << 0;
				const unsigned char DOWN = 1u << 1;
				const unsigned char LEFT = 1u << 2;
				const unsigned char RIGHT = 1u << 3;
			}
		}

		class KeyInputManager::Impl {
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
			const static std::unordered_map<Key::type, int> _mykey_dxkey_map;
			const static std::unordered_map<Pad::type, int> _mypad_dxpad_map;
			const static std::unordered_map<MouseButton::type, int> _mymousebutton_dxmousebutton_map;
			const static std::unordered_map<Pad::type, int> _pov_map;
		public:
			std::unordered_map<Button::type, std::vector<Key::type>> _key_to_button_map;
			std::unordered_map<Button::type, std::vector<Pad::type>> _pad_to_button_map;
			std::unordered_map<std::string, Button::type> _alias_button_map;
			std::unordered_map<std::string, bool> _tag_invalid_flag;
			Impl();
			~Impl() = default;
			void SetDefaultKeyButtonMap();
			void SetDefaultPadButtonMap();
			void UpdateDxKeyInput();
			bool isPov(Pad::type p)const { return _pov_map.find(p) == _pov_map.end() ? false : true; }
			bool statePOV(Pad::type p)const { return (dx_pov_input & _pov_map.at(p)) ? true : false; } //ハットスイッチ処理(ハットスイッチでなかったらfalse)
			bool pushPOV(Pad::type p)const { return (dx_pov_input & ~dx_pov_input_pre & _pov_map.at(p)) ? true : false; } //ハットスイッチ処理(ハットスイッチでなかったらfalse)
			bool GetDxKeyState(Key::type k)const { return dx_key_input[_mykey_dxkey_map.at(k)] ? true : false; }
			bool GetDxKeyPush(Key::type k)const {
				int dk = _mykey_dxkey_map.at(k);
				return dx_key_input[dk] ? (dx_key_input_pre[dk] ? false : true) : false;
			}
			bool GetDxPadState(Pad::type p)const {
				if (isPov(p)) { return statePOV(p); }
				return (dx_pad_input & _mypad_dxpad_map.at(p)) ? true : false;
			}
			bool GetDxPadPush(Pad::type p)const {
				if (isPov(p)) { return pushPOV(p); }
				return (dx_pad_input & ~dx_pad_input_pre & _mypad_dxpad_map.at(p)) ? true : false;
			}
			bool GetDxMouseButtonState(MouseButton::type mb)const {
				return dx_mouse_button_input & _mymousebutton_dxmousebutton_map.at(mb) ? true : false;
			}
			bool GetDxMouseButtonPush(MouseButton::type mb)const {
				int dk = _mymousebutton_dxmousebutton_map.at(mb);
				return (dx_mouse_button_input & dk) ? ((dx_mouse_button_input_pre & dk) ? false : true) : false;
			}
			void AssignKeyToButton(Key::type k, Button::type b) {
				if (k == 0) { return; }
				auto it = _key_to_button_map.find(b);
				if (it == _key_to_button_map.end()) {
					it = (_key_to_button_map.insert(std::make_pair(b, std::vector<Key::type>()))).first;
				}
				std::vector<Key::type>& v = it->second;
				int i = 0;
				for (Key::type p = 1; i < sizeof(Key::type) * 8; ++i, p <<= 1) {
					if (k&p) { v.push_back(p); }
				}
			}
			int GetDxMouseWheelRotation()const { return dx_mouse_wheel_rotation; }

			void AssignPadToButton(Pad::type p, Button::type b) {
				if (p == 0) { return; }
				auto it = _pad_to_button_map.find(b);
				if (it == _pad_to_button_map.end()) {
					it = (_pad_to_button_map.insert(std::make_pair(b, std::vector<Pad::type>()))).first;
				}
				std::vector<Pad::type>& v = it->second;
				int i = 0;
				for (Pad::type pp = 1; i < sizeof(Pad::type) * 8; ++i, pp <<= 1) {
					if (p&pp) { v.push_back(pp); }
				}
			}
		};
		const std::unordered_map<Key::type, int> KeyInputManager::Impl::_mykey_dxkey_map = {
			{Key::A, KEY_INPUT_A},
			{Key::B, KEY_INPUT_B},
			{Key::C, KEY_INPUT_C},
			{Key::D, KEY_INPUT_D},
			{Key::E, KEY_INPUT_E},
			{Key::F, KEY_INPUT_F},
			{Key::G, KEY_INPUT_G},
			{Key::H, KEY_INPUT_H},
			{Key::I, KEY_INPUT_I},
			{Key::J, KEY_INPUT_J},
			{Key::K, KEY_INPUT_K},
			{Key::L, KEY_INPUT_L},
			{Key::M, KEY_INPUT_M},
			{Key::N, KEY_INPUT_N},
			{Key::O, KEY_INPUT_O},
			{Key::P, KEY_INPUT_P},
			{Key::Q, KEY_INPUT_Q},
			{Key::R, KEY_INPUT_R},
			{Key::S, KEY_INPUT_S},
			{Key::T, KEY_INPUT_T},
			{Key::U, KEY_INPUT_U},
			{Key::V, KEY_INPUT_V},
			{Key::W, KEY_INPUT_W},
			{Key::X, KEY_INPUT_X},
			{Key::Y, KEY_INPUT_Y},
			{Key::Z, KEY_INPUT_Z},
			{Key::SPACE, KEY_INPUT_SPACE},
			{Key::LSHIFT, KEY_INPUT_LSHIFT},
			{Key::ENTER, KEY_INPUT_RETURN},
			{Key::LCONTROL, KEY_INPUT_LCONTROL},
			{Key::NUM_0, KEY_INPUT_0},
			{Key::NUM_1, KEY_INPUT_1},
			{Key::NUM_2, KEY_INPUT_2},
			{Key::NUM_3, KEY_INPUT_3},
			{Key::NUM_4, KEY_INPUT_4},
			{Key::NUM_5, KEY_INPUT_5},
			{Key::NUM_6, KEY_INPUT_6},
			{Key::NUM_7, KEY_INPUT_7},
			{Key::NUM_8, KEY_INPUT_8},
			{Key::NUM_9, KEY_INPUT_9},
			{Key::F1, KEY_INPUT_F1},
			{Key::F2, KEY_INPUT_F2},
			{Key::F3, KEY_INPUT_F3},
			{Key::F4, KEY_INPUT_F4},
			{Key::F5, KEY_INPUT_F5},
			{Key::F6, KEY_INPUT_F6},
			{Key::F7, KEY_INPUT_F7},
			{Key::F8, KEY_INPUT_F8},
			{Key::F9, KEY_INPUT_F9},
			{Key::F10, KEY_INPUT_F10},
			{Key::F11, KEY_INPUT_F11},
			{Key::F12, KEY_INPUT_F12},
			{Key::ESC, KEY_INPUT_ESCAPE},
			{Key::RSHIFT, KEY_INPUT_RSHIFT},
			{Key::RCONTROL, KEY_INPUT_RCONTROL},
			{Key::UP, KEY_INPUT_UP},
			{Key::DOWN, KEY_INPUT_DOWN},
			{Key::LEFT, KEY_INPUT_LEFT},
			{Key::RIGHT, KEY_INPUT_RIGHT},
		};
		const std::unordered_map<Pad::type, int> KeyInputManager::Impl::_mypad_dxpad_map = {
			{Pad::UP, PAD_INPUT_UP},
			{Pad::DOWN, PAD_INPUT_DOWN},
			{Pad::LEFT, PAD_INPUT_LEFT},
			{Pad::RIGHT, PAD_INPUT_RIGHT},
			{Pad::INPUT_1, PAD_INPUT_1},
			{Pad::INPUT_2, PAD_INPUT_2},
			{Pad::INPUT_3, PAD_INPUT_3},
			{Pad::INPUT_4, PAD_INPUT_4},
			{Pad::INPUT_5, PAD_INPUT_5},
			{Pad::INPUT_6, PAD_INPUT_6},
			{Pad::INPUT_7, PAD_INPUT_7},
			{Pad::INPUT_8, PAD_INPUT_8},
			{Pad::INPUT_9, PAD_INPUT_9},
			{Pad::INPUT_10, PAD_INPUT_10},
			{Pad::INPUT_11, PAD_INPUT_11},
			{Pad::INPUT_12, PAD_INPUT_12},
			{Pad::INPUT_13, PAD_INPUT_13},
			{Pad::INPUT_14, PAD_INPUT_14},
			{Pad::INPUT_15, PAD_INPUT_15},
			{Pad::INPUT_16, PAD_INPUT_16},
			{Pad::INPUT_17, PAD_INPUT_17},
			{Pad::INPUT_18, PAD_INPUT_18},
			{Pad::INPUT_19, PAD_INPUT_19},
			{Pad::INPUT_20, PAD_INPUT_20},
		};
		const std::unordered_map<MouseButton::type, int> KeyInputManager::Impl::_mymousebutton_dxmousebutton_map = {
			{ MouseButton::LEFT,MOUSE_INPUT_LEFT },
			{ MouseButton::RIGHT,MOUSE_INPUT_RIGHT },
			{ MouseButton::MIDDLE,MOUSE_INPUT_MIDDLE },
			{ MouseButton::INPUT_4,MOUSE_INPUT_4 },
			{ MouseButton::INPUT_5,MOUSE_INPUT_5 },
			{ MouseButton::INPUT_6,MOUSE_INPUT_6 },
			{ MouseButton::INPUT_7,MOUSE_INPUT_7 },
			{ MouseButton::INPUT_8,MOUSE_INPUT_8 },
		};
		const std::unordered_map<Pad::type, int> KeyInputManager::Impl::_pov_map = {
			{Pad::POV_UP, MY_POV_DIR::UP },
			{Pad::POV_DOWN, MY_POV_DIR::DOWN},
			{Pad::POV_LEFT, MY_POV_DIR::LEFT},
			{Pad::POV_RIGHT, MY_POV_DIR::RIGHT},
		};
		KeyInputManager::Impl::Impl() :dx_pad_input(0), dx_pad_input_pre(0), dx_pov_input(0), dx_pov_input_pre(0) {
			for (int i = 0; i < 256; ++i) {
				dx_key_input_pre[i] = 0;
				dx_key_input[i] = 0;
			}
		}
		void KeyInputManager::Impl::UpdateDxKeyInput() {
			//マウスホイール
			dx_mouse_wheel_rotation = GetMouseWheelRotVol();
			//マウスボタン
			dx_mouse_button_input_pre = dx_mouse_button_input;
			dx_mouse_button_input = GetMouseInput();
			//キーボード
			for (int i = 0; i < 256; ++i) {
				dx_key_input_pre[i] = dx_key_input[i];
			}
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

				if (dj.POV[0] == 0xffffffff) { dx_pov_input = 0; }
				else {
					int dn = (dj.POV[0] + 2250) % 36000 / 4500;
					switch (dn) {
					case 0:dx_pov_input = MY_POV_DIR::UP; break;
					case 1:dx_pov_input = MY_POV_DIR::UP | MY_POV_DIR::RIGHT; break;
					case 2:dx_pov_input = MY_POV_DIR::RIGHT; break;
					case 3:dx_pov_input = MY_POV_DIR::RIGHT | MY_POV_DIR::DOWN; break;
					case 4:dx_pov_input = MY_POV_DIR::DOWN; break;
					case 5:dx_pov_input = MY_POV_DIR::DOWN | MY_POV_DIR::LEFT; break;
					case 6:dx_pov_input = MY_POV_DIR::LEFT; break;
					case 7:dx_pov_input = MY_POV_DIR::LEFT | MY_POV_DIR::UP; break;
					}
				}
			}
		}
		void KeyInputManager::Impl::SetDefaultKeyButtonMap() {
			AssignKeyToButton(Key::UP, Button::UP);
			AssignKeyToButton(Key::DOWN, Button::DOWN);
			AssignKeyToButton(Key::LEFT, Button::LEFT);
			AssignKeyToButton(Key::RIGHT, Button::RIGHT);
			AssignKeyToButton(Key::Z, Button::A);
			AssignKeyToButton(Key::X, Button::B);
			AssignKeyToButton(Key::A, Button::X);
			AssignKeyToButton(Key::S, Button::Y);
			AssignKeyToButton(Key::SPACE, Button::START);
			AssignKeyToButton(Key::LSHIFT, Button::L);
			AssignKeyToButton(Key::LCONTROL, Button::R);
		}
		void KeyInputManager::Impl::SetDefaultPadButtonMap() {
			AssignPadToButton(Pad::UP | Pad::POV_UP, Button::UP);
			AssignPadToButton(Pad::DOWN | Pad::POV_DOWN, Button::DOWN);
			AssignPadToButton(Pad::LEFT | Pad::POV_LEFT, Button::LEFT);
			AssignPadToButton(Pad::RIGHT | Pad::POV_RIGHT, Button::RIGHT);
			AssignPadToButton(Pad::INPUT_3, Button::A);
			AssignPadToButton(Pad::INPUT_4, Button::B);
			AssignPadToButton(Pad::INPUT_1, Button::X);
			AssignPadToButton(Pad::INPUT_2, Button::Y);
			AssignPadToButton(Pad::INPUT_12, Button::START);
			AssignPadToButton(Pad::INPUT_5, Button::L);
			AssignPadToButton(Pad::INPUT_6, Button::R);
		}

		KeyInputManager::KeyInputManager() :_impl(std::make_unique<Impl>()) {
		}

		bool KeyInputManager::Initialize() {
			return true;
		}

		void KeyInputManager::ResetAllSettings() {
			ResetKeyPadButtonMap();
			ResetButtonAlias();
			ResetInvalidTag();
		}

		void KeyInputManager::ResetKeyPadButtonMap() {
			_impl->_key_to_button_map.clear();
			_impl->_pad_to_button_map.clear();
		}

		void KeyInputManager::ResetButtonAlias() {
			_impl->_alias_button_map.clear();
		}

		void KeyInputManager::SetDefaultSettings() {
			_impl->SetDefaultKeyButtonMap();
			_impl->SetDefaultPadButtonMap();
		}

		bool KeyInputManager::Update() {
			_impl->UpdateDxKeyInput();
			return true;
		}

		bool KeyInputManager::KeyState(Key::type k, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			auto tit = _impl->_tag_invalid_flag.find(tag);
			if (tit != _impl->_tag_invalid_flag.end() && tit->second) { return false; }

			return _impl->GetDxKeyState(k);
		}

		bool KeyInputManager::KeyPush(Key::type k, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);

			return _impl->GetDxKeyPush(k);
		}

		bool KeyInputManager::PadState(Pad::type k, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);

			return _impl->GetDxPadState(k);
		}

		bool KeyInputManager::PadPush(Pad::type k, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);

			return _impl->GetDxPadPush(k);
		}

		bool KeyInputManager::ButtonState(Button::type b, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);
			//キー
			auto kit = _impl->_key_to_button_map.find(b);
			if (kit != _impl->_key_to_button_map.end()) {
				for (Key::type k : kit->second) {
					if (KeyState(k)) { return true; }
				}
			}
			//パッド
			auto pit = _impl->_pad_to_button_map.find(b);
			if (pit != _impl->_pad_to_button_map.end()) {
				for (Pad::type k : pit->second) {
					if (PadState(k)) { return true; }
				}
			}
			return false;
		}

		bool KeyInputManager::ButtonPush(Button::type b, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);
			//キー
			auto kit = _impl->_key_to_button_map.find(b);
			if (kit != _impl->_key_to_button_map.end()) {
				for (Key::type k : kit->second) {
					if (KeyPush(k)) { return true; }
				}
			}
			//パッド
			auto pit = _impl->_pad_to_button_map.find(b);
			if (pit != _impl->_pad_to_button_map.end()) {
				for (Pad::type k : pit->second) {
					if (PadPush(k)) { return true; }
				}
			}
			return false;
		}

		bool KeyInputManager::ButtonState(const std::string& a, const std::string& tag)const {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) { return false; }
			return ButtonState(it->second, tag);
		}

		bool KeyInputManager::ButtonPush(const std::string& a, const std::string& tag)const {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) { return false; }
			return ButtonPush(it->second, tag);
		}

		bool KeyInputManager::MouseButtonState(MouseButton::type mb, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			auto tit = _impl->_tag_invalid_flag.find(tag);
			if (tit != _impl->_tag_invalid_flag.end() && tit->second) { return false; }

			return _impl->GetDxMouseButtonState(mb);
		}

		bool KeyInputManager::MouseButtonPush(MouseButton::type mb, const std::string& tag)const {
			//指定タグの無効かフラグがtrueだったらスキップ
			TAG_CHECK_PROC(tag);

			return _impl->GetDxMouseButtonPush(mb);
		}

		void KeyInputManager::ValidateInputByTag(const std::string& tag) {
			if (tag == "\0") { return; }
			auto it = _impl->_tag_invalid_flag.find(tag);
			if (it == _impl->_tag_invalid_flag.end()) {
				_impl->_tag_invalid_flag.insert(make_pair(tag, false));
			}
			else { it->second = false; }
		}

		void KeyInputManager::InvalidateInputByTag(const std::string& tag) {
			if (tag == "\0") { return; }
			auto it = _impl->_tag_invalid_flag.find(tag);
			if (it == _impl->_tag_invalid_flag.end()) {
				_impl->_tag_invalid_flag.insert(make_pair(tag, true));
			}
			else { it->second = true; }
		}

		bool KeyInputManager::IsInputInvalidByTag(const std::string& tag)const {
			if (tag == "\0") { return false; }
			auto it = _impl->_tag_invalid_flag.find(tag);
			if (it == _impl->_tag_invalid_flag.end()) { return true; }
			return it->second;
		}

		void KeyInputManager::ResetInvalidTag() { _impl->_tag_invalid_flag.clear(); }

		void KeyInputManager::AssignAliasToButton(const std::string& a, Button::type b) {
			auto it = _impl->_alias_button_map.find(a);
			if (it == _impl->_alias_button_map.end()) {
				it = (_impl->_alias_button_map.insert(std::make_pair(a, b))).first;
			}
			it->second = b;
		}

		void KeyInputManager::AssignKeyToButton(Key::type k, Button::type b) {
			_impl->AssignKeyToButton(k, b);
		}

		void KeyInputManager::AssignPadToButton(Pad::type p, Button::type b) {
			_impl->AssignPadToButton(p, b);
		}

		const std::unordered_map<Button::type, std::vector<Key::type>>& KeyInputManager::GetAssignedKeyToButtonMap()const {
			return _impl->_key_to_button_map;
		}

		const std::unordered_map<Button::type, std::vector<Pad::type>>& KeyInputManager::GetAssignedPadToButtonMap()const {
			return _impl->_pad_to_button_map;
		}

		KeyInputManager::~KeyInputManager(){}

		const Vector2Di KeyInputManager::GetMousePointerPosition() const
		{
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

		int KeyInputManager::GetMouseWheelRotation() const
		{
			return _impl->GetDxMouseWheelRotation();
		}

		void KeyInputManager::Finalize() {
		}

	}
}

#undef TAG_CHECK_PROC
