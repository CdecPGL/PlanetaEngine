#pragma once

#include <unordered_map>
#include <vector>
#include "IInputManager.hpp"
#include "SubSystemManager.hpp"

namespace plnt {
	/*インプットマネージャ*/
	namespace private_ {
		class InputManager : public IInputManager, public SubSystemManager {
		public:
			virtual ~InputManager() override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual bool Update() = 0;
			/*すべての設定をリセットする*/
			virtual void ResetAllSettings() = 0;
			/*キーとパッドのボタン割り当てをリセットする*/
			virtual void ResetKeyPadButtonMap() = 0;
			/*ボタンの別名をリセットする*/
			virtual void ResetButtonAlias() = 0;
			/*デフォルトの設定値に戻す*/
			virtual void SetDefaultSettings() = 0;
			/*ボタンにキーを割り当てる(キーは複数指定可)*/
			virtual void AssignKeyToButton(Key::type, Button::type) = 0;
			/*ボタンにパッド入力を割り当てる(パッドは複数指定可)*/
			virtual void AssignPadToButton(Pad::type, Button::type) = 0;
			/*ボタンに別名をつける。既存の場合は割り当てなおす(複数指定不可)*/
			virtual void AssignAliasToButton(const std::string &, Button::type) = 0;
			/*ボタンに割り当てられたキーマップ取得*/
			virtual const std::unordered_map<Button::type, std::vector<Key::type>> &GetAssignedKeyToButtonMap() const =
			0;
			/*ボタンに割り当てられたキーマップ取得*/
			virtual const std::unordered_map<Button::type, std::vector<Pad::type>> &GetAssignedPadToButtonMap() const =
			0;
		};
	}
}
