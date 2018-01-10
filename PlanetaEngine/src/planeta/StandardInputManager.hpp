#pragma once

#include<memory>
#include "planeta/InputManager.hpp"

namespace plnt {
	namespace private_ {
		class StandardInputManager final : public InputManager {
		public:
			StandardInputManager();
			~StandardInputManager();
			/*初期化*/
			bool Initialize()override;
			/*終了処理*/
			void Finalize()override;
			/*更新*/
			bool Update()override;
			/*すべての設定をリセットする*/
			void ResetAllSettings()override;
			/*キーとパッドのボタン割り当てをリセットする*/
			void ResetKeyPadButtonMap()override;
			/*ボタンの別名をリセットする*/
			void ResetButtonAlias()override;
			/*デフォルトの設定値に戻す*/
			void SetDefaultSettings()override;
			/*@brief キーの入力状態を取得(複数指定不可)
			@param キーコード*/
			bool KeyState(Key::type)const override;
			/*@brief パッドの入力状態を取得(複数指定不可)
			@param パッドコード*/
			bool PadState(Pad::type)const override;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタンコード*/
			bool ButtonState(Button::type)const override;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタン名*/
			bool ButtonState(const std::string&)const override;
			/*@brief マウスボタンの入力状態を取得(複数指定不可)
			@param マウスボタンコード*/
			bool MouseButtonState(MouseButton::type)const override;
			/*@brief キーの押下取得(複数指定不可)
			@param キーコード*/
			bool KeyPush(Key::type)const override;
			/*@brief パッドの押下取得(複数指定不可)
			@param パッドコード*/
			bool PadPush(Pad::type)const override;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタンコード*/
			bool ButtonPush(Button::type)const override;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタン名*/
			bool ButtonPush(const std::string&)const override;
			/*@brief マウスボタンの押下取得(複数指定不可)
			@param マウスボタンコード*/
			bool MouseButtonPush(MouseButton::type)const override;
			/*ボタンにキーを割り当てる(キーは複数指定可)*/
			void AssignKeyToButton(Key::type, Button::type)override;
			/*ボタンにパッド入力を割り当てる(パッドは複数指定可)*/
			void AssignPadToButton(Pad::type, Button::type)override;
			/*ボタンに別名をつける。既存の場合は割り当てなおす(複数指定不可)*/
			void AssignAliasToButton(const std::string&, Button::type)override;
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<Button::type, std::vector<Key::type>>& GetAssignedKeyToButtonMap()const override;
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<Button::type, std::vector<Pad::type>>& GetAssignedPadToButtonMap()const override;
			/*マウスポインタの位置を取得*/
			const Vector2Di GetMousePointerPosition()const override;
			/*マウスホイールの回転量を取得*/
			int GetMouseWheelRotation()const override;
		private:
			StandardInputManager(const StandardInputManager&) = delete;
			StandardInputManager(StandardInputManager&&) = delete;
			const StandardInputManager& operator = (const StandardInputManager&) = delete;
			const StandardInputManager& operator = (StandardInputManager&&) = delete;
			class Impl;
			std::unique_ptr<Impl> _impl;
		};
	}
}
