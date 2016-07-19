#pragma once

#include <memory>
#include <unordered_map>
#include <functional>

namespace planeta {
	class GameObjectBase;
	namespace core {
		class GameObjectTemplateHolder {
		public:
			/*新しいゲームオブジェクトを取得する(file_idがemptyの場合はファイルの読み込みを行わない)*/
			std::shared_ptr<GameObjectBase> GetNewGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc);
		private:
			std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<GameObjectBase>>> game_object_templates_;
			/*ゲームオブジェクトの作成*/
			std::shared_ptr<GameObjectBase> CreateGameObject_(const std::string& game_object_type_id, const std::string& file_id, const std::function<void(GameObjectBase&)>& post_instantiate_proc);
		};
	}
}
