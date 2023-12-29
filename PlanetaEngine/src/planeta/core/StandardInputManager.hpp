#pragma once

#include<memory>
#include "input_manager.hpp"

namespace plnt {
	namespace private_ {
		class StandardInputManager final : public input_manager {
		public:
			StandardInputManager();
			~StandardInputManager();
			/*初期化*/
			bool initialize() override;
			/*終了処理*/
			void finalize() override;
			/*更新*/
			bool update() override;
			/*すべての設定をリセットする*/
			void reset_all_settings() override;
			/*キーとパッドのボタン割り当てをリセットする*/
			void reset_key_pad_button_map() override;
			/*ボタンの別名をリセットする*/
			void reset_button_alias() override;
			/*デフォルトの設定値に戻す*/
			void set_default_settings() override;
			/*@brief キーの入力状態を取得(複数指定不可)
			@param キーコード*/
			bool key_state(keyboard::type) const override;
			/*@brief パッドの入力状態を取得(複数指定不可)
			@param パッドコード*/
			bool pad_state(pad::type) const override;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタンコード*/
			bool button_state(button::type) const override;
			/*@brief ボタンの入力状態を取得(複数指定不可)
			@param ボタン名*/
			bool button_state(const std::string &) const override;
			/*@brief マウスボタンの入力状態を取得(複数指定不可)
			@param マウスボタンコード*/
			bool mouse_button_state(mouse_button::type) const override;
			/*@brief キーの押下取得(複数指定不可)
			@param キーコード*/
			bool key_push(keyboard::type) const override;
			/*@brief パッドの押下取得(複数指定不可)
			@param パッドコード*/
			bool pad_push(pad::type) const override;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタンコード*/
			bool button_push(button::type) const override;
			/*@brief ボタンの押下取得(複数指定不可)
			@param ボタン名*/
			bool button_push(const std::string &) const override;
			/*@brief マウスボタンの押下取得(複数指定不可)
			@param マウスボタンコード*/
			bool mouse_button_push(mouse_button::type) const override;
			/*ボタンにキーを割り当てる(キーは複数指定可)*/
			void assign_key_to_button(keyboard::type, button::type) override;
			/*ボタンにパッド入力を割り当てる(パッドは複数指定可)*/
			void assign_pad_to_button(pad::type, button::type) override;
			/*ボタンに別名をつける。既存の場合は割り当てなおす(複数指定不可)*/
			void assign_alias_to_button(const std::string &, button::type) override;
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<button::type, std::vector<keyboard::type>> &get_assigned_key_to_button_map() const override;
			/*ボタンに割り当てられたキーマップ取得*/
			const std::unordered_map<button::type, std::vector<pad::type>> &get_assigned_pad_to_button_map() const override;
			/*マウスポインタの位置を取得*/
			const Vector2Di get_mouse_pointer_position() const override;
			/*マウスホイールの回転量を取得*/
			int get_mouse_wheel_rotation() const override;

		private:
			StandardInputManager(const StandardInputManager &) = delete;
			StandardInputManager(StandardInputManager &&) = delete;
			const StandardInputManager &operator =(const StandardInputManager &) = delete;
			const StandardInputManager &operator =(StandardInputManager &&) = delete;
			class Impl;
			std::unique_ptr<Impl> _impl;
		};
	}
}
