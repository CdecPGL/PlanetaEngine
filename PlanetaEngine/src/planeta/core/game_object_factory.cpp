#include "boost/algorithm/string.hpp"

#include "game.hpp"
#include "i_resource_manager.hpp"
#include "game_object_factory.hpp"
#include "planeta/reflection/reflection.hpp"
#include "game_object_base.hpp"
#include "planeta/buildin/RGameObject.hpp"

namespace plnt::private_ {
	std::shared_ptr<game_object_base> game_object_factory::get_new_game_object(
		const std::string &game_object_resource_id, const WeakPointer<i_scene_internal> &scene_data) {
		//テンプレート
		std::shared_ptr<game_object_base> go_temp;
		//必要ならテンプレートを作成登録し、使用するテンプレートをセットする
		if (const auto gt_it = game_object_templates_.find(game_object_resource_id); gt_it == game_object_templates_.
			end()) {
			go_temp = create_game_object_from_resource(game_object_resource_id, scene_data);
			if (go_temp == nullptr) { return nullptr; }
			game_object_templates_.emplace(game_object_resource_id, go_temp);
		} else { go_temp = gt_it->second; }
		assert(go_temp != nullptr);
		//テンプレートをクローンし、返す
		auto ngo = clone_game_object_from_template(go_temp, scene_data);
		if (ngo == nullptr) {
			PE_LOG_ERROR("GameObjectテンプレート(GameObject定義ファイルID:\"", game_object_resource_id, "\")からのクローン作製に失敗しました。");
		}
		return ngo;
	}

	std::shared_ptr<game_object_base> game_object_factory::get_new_game_object(
		const std::vector<std::string> &game_object_component_type_id_list,
		const WeakPointer<i_scene_internal> &scene_data) const {
		return create_game_object_from_component_type_list(game_object_component_type_id_list, scene_data);
	}

	std::shared_ptr<game_object_base> game_object_factory::create_game_object_from_component_type_list(
		const std::vector<std::string> &game_object_component_type_id_list,
		const WeakPointer<i_scene_internal> &scene_data) const {
		auto go_info = util::ConvertAndConnectToString("ゲームオブジェクトコンポーネントリスト:",
		                                               boost::algorithm::join(game_object_component_type_id_list, ","));
		//生成
		auto ngo = std::make_shared<game_object_base>();
		if (ngo == nullptr) {
			PE_LOG_ERROR("ゲームオブジェクト(", go_info, ")の作成に失敗しました。");
			return nullptr;
		}
		//シーンデータセット
		ngo->set_scene_internal_interface(scene_data);
		//コンポーネントの追加と設定
		if (!ngo->add_and_set_up_components(game_object_component_type_id_list)) {
			PE_LOG_ERROR("ゲームオブジェクト(", go_info, ")のコンポーネントの追加または設定に失敗しました。");
			return nullptr;
		}
		//成功
		return ngo;
	}

	std::shared_ptr<game_object_base> game_object_factory::create_game_object_from_resource(
		const std::string &game_object_resource_id, const WeakPointer<i_scene_internal> &scene_data) const {
		//生成
		auto ngo = std::make_shared<game_object_base>();
		if (ngo == nullptr) {
			PE_LOG_ERROR("ゲームオブジェクト(リソースID:\"", game_object_resource_id, "\")の作成に失敗しました。");
			return nullptr;
		}
		//シーンデータセット
		ngo->set_scene_internal_interface(scene_data);
		//ファイル読み込み
		const auto go_res = game::instance().resource_manager()->get_resource_by_id<RGameObject>(game_object_resource_id);
		if (go_res == nullptr) {
			PE_LOG_ERROR("ゲームオブジェクト定義リソース\"", game_object_resource_id, "\"の読み込みに失敗しました。");
			return nullptr;
		}
		//コンポーネントの追加と設定
		if (!ngo->add_and_set_up_components(*go_res->GetPtree())) {
			PE_LOG_ERROR("ゲームオブジェクトへリソース\"", game_object_resource_id, "\"を読み込むことができませんでした。");
			return nullptr;
		}
		//成功
		return ngo;
	}

	std::shared_ptr<game_object_base> game_object_factory::clone_game_object_from_template(
		const std::shared_ptr<game_object_base> &go_temp, const WeakPointer<i_scene_internal> &scene_data) const {
		assert(go_temp != nullptr);
		//生成
		// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
		auto ngo = reflection::reflection::create_object_by_std_type_info<game_object_base>(typeid(*go_temp));
		if (ngo == nullptr) {
			PE_LOG_ERROR("クローンの作成に失敗しました。");
			return nullptr;
		}
		//クローン時処理
		if (!ngo->process_cloning(go_temp)) { PE_LOG_ERROR("ゲームオブジェクトのクローン時処理に失敗しました。"); }
		//成功
		return ngo;
	}
}
