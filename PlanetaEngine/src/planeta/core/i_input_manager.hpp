#pragma once

#include <string>

#include "Vector2D.hpp"
#include "input_definitions.hpp"

namespace plnt {
	/*! インプットマネージャの公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_input_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_input_manager() = 0 { }

		/*@brief キーの入力状態を取得(複数指定不可)
		@param キーコード*/
		[[nodiscard]] virtual bool key_state(keyboard::type) const = 0;
		/*@brief パッドの入力状態を取得(複数指定不可)
		@param パッドコード*/
		[[nodiscard]] virtual bool pad_state(pad::type) const = 0;
		/*@brief ボタンの入力状態を取得(複数指定不可)
		@param ボタンコード*/
		[[nodiscard]] virtual bool button_state(button::type) const = 0;
		/*@brief ボタンの入力状態を取得(複数指定不可)
		@param ボタン名*/
		[[nodiscard]] virtual bool button_state(const std::string &) const = 0;
		/*@brief マウスボタンの入力状態を取得(複数指定不可)
		@param マウスボタンコード*/
		[[nodiscard]] virtual bool mouse_button_state(mouse_button::type) const = 0;
		/*@brief キーの押下取得(複数指定不可)
		@param キーコード*/
		[[nodiscard]] virtual bool key_push(keyboard::type) const = 0;
		/*@brief パッドの押下取得(複数指定不可)
		@param パッドコード*/
		[[nodiscard]] virtual bool pad_push(pad::type) const = 0;
		/*@brief ボタンの押下取得(複数指定不可)
		@param ボタンコード*/
		[[nodiscard]] virtual bool button_push(button::type) const = 0;
		/*@brief ボタンの押下取得(複数指定不可)
		@param ボタン名*/
		[[nodiscard]] virtual bool button_push(const std::string &) const = 0;
		/*@brief マウスボタンの押下取得(複数指定不可)
		@param マウスボタンコード*/
		[[nodiscard]] virtual bool mouse_button_push(mouse_button::type) const = 0;
		/*! マウスポインタの位置を取得*/
		[[nodiscard]] virtual const vector_2di get_mouse_pointer_position() const = 0;
		/*! マウスホイールの回転量を取得*/
		[[nodiscard]] virtual int get_mouse_wheel_rotation() const = 0;
	};
}
