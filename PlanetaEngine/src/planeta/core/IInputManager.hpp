#pragma once

#include <string>
#include "Vector2D.hpp"
#include "InputDefinitions.hpp"

namespace plnt {
	/*! インプットマネージャの公開インターフェイス*/
	class IInputManager {
	public:
		virtual ~IInputManager() = 0 { };
		/*@brief キーの入力状態を取得(複数指定不可)
		@param キーコード*/
		virtual bool KeyState(Key::type) const = 0;
		/*@brief パッドの入力状態を取得(複数指定不可)
		@param パッドコード*/
		virtual bool PadState(Pad::type) const = 0;
		/*@brief ボタンの入力状態を取得(複数指定不可)
		@param ボタンコード*/
		virtual bool ButtonState(Button::type) const = 0;
		/*@brief ボタンの入力状態を取得(複数指定不可)
		@param ボタン名*/
		virtual bool ButtonState(const std::string &) const = 0;
		/*@brief マウスボタンの入力状態を取得(複数指定不可)
		@param マウスボタンコード*/
		virtual bool MouseButtonState(MouseButton::type) const = 0;
		/*@brief キーの押下取得(複数指定不可)
		@param キーコード*/
		virtual bool KeyPush(Key::type) const = 0;
		/*@brief パッドの押下取得(複数指定不可)
		@param パッドコード*/
		virtual bool PadPush(Pad::type) const = 0;
		/*@brief ボタンの押下取得(複数指定不可)
		@param ボタンコード*/
		virtual bool ButtonPush(Button::type) const = 0;
		/*@brief ボタンの押下取得(複数指定不可)
		@param ボタン名*/
		virtual bool ButtonPush(const std::string &) const = 0;
		/*@brief マウスボタンの押下取得(複数指定不可)
		@param マウスボタンコード*/
		virtual bool MouseButtonPush(MouseButton::type) const = 0;
		/*! マウスポインタの位置を取得*/
		virtual const Vector2Di GetMousePointerPosition() const = 0;
		/*! マウスホイールの回転量を取得*/
		virtual int GetMouseWheelRotation() const = 0;
	};
}
