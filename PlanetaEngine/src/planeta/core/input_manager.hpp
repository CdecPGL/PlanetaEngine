#pragma once

#include <unordered_map>
#include <vector>

#include "i_input_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	/*インプットマネージャ*/
	namespace private_ {
		// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		class input_manager : public i_input_manager, public SubSystemManager {
		public:
			// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
			~input_manager() override = 0 {}

			virtual bool initialize() = 0;
			virtual void finalize() = 0;
			virtual bool update() = 0;
			/*すべての設定をリセットする*/
			virtual void reset_all_settings() = 0;
			/*キーとパッドのボタン割り当てをリセットする*/
			virtual void reset_key_pad_button_map() = 0;
			/*ボタンの別名をリセットする*/
			virtual void reset_button_alias() = 0;
			/*デフォルトの設定値に戻す*/
			virtual void set_default_settings() = 0;
			/*ボタンにキーを割り当てる(キーは複数指定可)*/
			virtual void assign_key_to_button(keyboard::type, button::type) = 0;
			/*ボタンにパッド入力を割り当てる(パッドは複数指定可)*/
			virtual void assign_pad_to_button(pad::type, button::type) = 0;
			/*ボタンに別名をつける。既存の場合は割り当てなおす(複数指定不可)*/
			virtual void assign_alias_to_button(const std::string &, button::type) = 0;
			/*ボタンに割り当てられたキーマップ取得*/
			[[nodiscard]] virtual const std::unordered_map<button::type, std::vector<keyboard::type>> &
			get_assigned_key_to_button_map() const =0;
			/*ボタンに割り当てられたキーマップ取得*/
			[[nodiscard]] virtual const std::unordered_map<button::type, std::vector<pad::type>> &
			get_assigned_pad_to_button_map() const =0;
		};
	}
}
