#pragma once

#include "Object.h"
#include <string>
#include <unordered_map>
#include<memory>
#include <vector>
#include"KeyInputDefines.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core {
		class KeyInputManager final {
		public:
			KeyInputManager();
			~KeyInputManager();
			/*初期化*/
			bool Initialize();
			/*更新*/
			bool Update();
			/*すべての設定をリセットする*/
			void ResetAllSettings();
			/*キーとパッドのボタン割り当てをリセットする*/
			void ResetKeyPadButtonMap();
			/*ボタンの別名をリセットする*/
			void ResetButtonAlias();
			/*デフォルトの設定値に戻す*/
			void SetDefaultSettings();
			/*@brief キーの入力状態を取得(複数指定不可)
			@param キーコード、タグ*/
			bool KeyState(Key::type, const std::string& = "\0")const;
			/*@brief パッドの入力状態を取得(複数指定不可)
			@param パッドコード、タグ*/
			bool PadState(Pad::type, const std::string& = "\0")const;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタンコード、タグ*/
			bool ButtonState(Button::type, const std::string& = "\0")const;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタン名、タグ*/
			bool ButtonState(const std::string&, const std::string& = "\0")const;
			/*@brief マウスボタンの入力状態を取得(複数指定不可)
			@param マウスボタンコード、タグ*/
			bool MouseButtonState(MouseButton::type, const std::string& = "\0")const;
			/*@brief キーの押下取得(複数指定不可)
			@param キーコード、タグ*/
			bool KeyPush(Key::type, const std::string& = "\0")const;
			/*@brief パッドの押下取得(複数指定不可)
			@param パッドコード、タグ*/
			bool PadPush(Pad::type, const std::string& = "\0")const;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタンコード、タグ*/
			bool ButtonPush(Button::type, const std::string& = "\0")const;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタン名、タグ*/
			bool ButtonPush(const std::string&, const std::string& = "\0")const;
			/*@brief マウスボタンの押下取得(複数指定不可)
			@param マウスボタンコード、タグ*/
			bool MouseButtonPush(MouseButton::type, const std::string& = "\0")const;
			/*ボタンにキーを割り当てる(キーは複数指定可)*/
			void AssignKeyToButton(Key::type, Button::type);
			/*ボタンにパッド入力を割り当てる(パッドは複数指定可)*/
			void AssignPadToButton(Pad::type, Button::type);
			/*ボタンに別名をつける。既存の場合は割り当てなおす(複数指定不可)*/
			void AssignAliasToButton(const std::string&, Button::type);
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<Button::type, std::vector<Key::type>>& GetAssignedKeyToButtonMap()const;
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<Button::type, std::vector<Pad::type>>& GetAssignedPadToButtonMap()const;
			/*指定タグの入力を無効化*/
			void InvalidateInputByTag(const std::string&);
			/*指定タグの入力を有効化*/
			void ValidateInputByTag(const std::string&);
			/*指定タグの入力が無効か確認する*/
			bool IsInputInvalidByTag(const std::string&)const;
			/*タグの入力無効化をリセットする*/
			void ResetInvalidTag();
			/*マウスポインタの位置を取得*/
			const Vector2Di GetMousePointerPosition()const;
			/*マウスホイールの回転量を取得*/
			int GetMouseWheelRotation()const;
		private:
			KeyInputManager(const KeyInputManager&) = delete;
			KeyInputManager(KeyInputManager&&) = delete;
			const KeyInputManager& operator = (const KeyInputManager&) = delete;
			const KeyInputManager& operator = (KeyInputManager&&) = delete;
			class Impl;
			std::unique_ptr<Impl> _impl;
		};
	}
}
