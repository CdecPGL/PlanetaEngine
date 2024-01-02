#pragma once

#include <memory>
#include <unordered_map>

#include "WeakPointer.hpp"

namespace plnt::private_ {
	class i_scene_internal;
	class game_object_base;

	/// <summary>ゲームオブジェクトの生成クラス。</summary>
	class game_object_factory {
	public:
		/// <summary>ゲームオブジェクトリソースIDを指定して新しいゲームオブジェクトを取得する。</summary>
			/// <param name="game_object_resource_id">取得するゲームオブジェクトのリソースID</param>
			/// <param name="scene_data">対象のシーンデータ</param>
			/// <returns>新しいゲームオブジェクト</returns>
		std::shared_ptr<game_object_base> get_new_game_object(const std::string &game_object_resource_id,
		                                                   const weak_pointer<i_scene_internal> &scene_data);
		/// <summary>コンポーネントリストから新しいゲームオブジェクトを取得する。</summary>
			/// <param name="game_object_component_type_id_list">ゲームオブジェクトコンポーネントタイプIDのリスト</param>
			/// <param name="scene_data">対象のシーンデータ</param>
			/// <returns>新しいゲームオブジェクト</returns>
		[[nodiscard]] std::shared_ptr<game_object_base> get_new_game_object(
			const std::vector<std::string> &game_object_component_type_id_list,
			const weak_pointer<i_scene_internal> &scene_data) const;

	private:
		/// <summary>The game object templates.</summary>
		std::unordered_map<std::string, std::shared_ptr<game_object_base>> game_object_templates_;
		/// <summary>コンポーネントリストからゲームオブジェクトを作成する。</summary>
			/// <param name="game_object_component_type_id_list">ゲームオブジェクトコンポーネントタイプIDのリスト</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
		[[nodiscard]] std::shared_ptr<game_object_base> create_game_object_from_component_type_list(
			const std::vector<std::string> &game_object_component_type_id_list,
			const weak_pointer<i_scene_internal> &scene_data) const;
		/// <summary>ゲームオブジェクトリソースIDを指定してゲームオブジェクトを作成する。</summary>
			/// <param name="game_object_resource_id">作成するゲームオブジェクトのリソースID</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
		[[nodiscard]] std::shared_ptr<game_object_base> create_game_object_from_resource(
			const std::string &game_object_resource_id, const weak_pointer<i_scene_internal> &scene_data) const;
		/// <summary>ゲームオブジェクトのクローンをゲームオブジェクトテンプレートから作成する。</summary>
			/// <param name="go_temp">作成元のテンプレート</param>
			/// <param name="scene_data">作成先のシーンデータ</param>
			/// <returns>作成されたゲームオブジェクト</returns>
		[[nodiscard]] std::shared_ptr<game_object_base> clone_game_object_from_template(
			const std::shared_ptr<game_object_base> &go_temp,
			const weak_pointer<i_scene_internal> &scene_data) const;
	};
}
