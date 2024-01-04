#include "resource_manager.hpp"
#include "standard_scene_manager.hpp"
#include "i_debug_manager.hpp"
#include "scene.hpp"
#include "scene_setupper.hpp"
#include "..\buildin\s_error.hpp"
#include "log_utility.hpp"
#include "null_weak_pointer_exception.hpp"
#include "prefix_utility.hpp"
#include "standard_scene_manager_utility.hpp"

namespace plnt::private_ {
	standard_scene_manager::standard_scene_manager() : state_(state::none), is_transitioning_(false),
	                                               is_loading_(false), is_next_scene_loaded_(false),
	                                               load_progress_(0.0) { }


	scene_status standard_scene_manager::process() {
		try {
			switch (state_) {
				case state::none:
					PE_LOG_WARNING("シーンが開始されていません。");
					break;
				case state::transition_requested:
					transition_proc();
				case state::progress:
					current_scene_->update();
					return scene_status::play;
				case state::quit_requested:
					return scene_status::quit;
				case state::error_occured:
					return scene_status::error;
			}
			return scene_status::error;
		} catch (null_weak_pointer_exception &e) {
			PE_LOG_ERROR("Scene::Updateで無効なWeakPointerが参照されました。", e.what());
			return scene_status::error;
		}
	}

	bool standard_scene_manager::load_next_scene(const std::string &scene_name) {
		if (is_loading() || is_transitioning()) {
			PE_LOG_ERROR("シーンの読み込みに失敗しました。シーンの読み込み中、または遷移中のため新たにシーン(", scene_name, ")を読み込むことはできません。");
			return false;
		} //すでに読み込み中か、遷移中のため、あらたに読み込みできない
		//シーン定義クラス作成
		next_scene_setupper_ = create_scene_set_upper(scene_name);
		//シーン定義クラスが作成できなかった
		if (next_scene_setupper_ == nullptr) {
			PE_LOG_ERROR("シーンの読み込みに失敗しました。指定されたシーン(", scene_name, ")のセットアッパーが存在しません。");
			return false;
		}
		//リソース読み込み
		//シーンIDと同じIDのタグをアンロード対象外に指定し、読み込む
		assert(resource_manager_ != nullptr);
		auto &rm = *resource_manager_;
		bool scc = rm.set_not_unload_tags({scene_name});
		scc &= rm.prepare_resources({scene_name});
		if (!scc) {
			PE_LOG_WARNING("指定されたシーン(", scene_name, ")のリソース準備に失敗しました。読み込みに失敗したか、対象のリソースが存在しない可能性があります。");
			//return false;
		} //リソースの準備ができなかった
		//準備完了
		next_scene_id_ = scene_name;
		is_next_scene_loaded_ = true;
		PE_LOG_MESSAGE("指定されたシーン(", scene_name, ")を読み込みました。");
		return true;
	}

	bool standard_scene_manager::transition_scene(const util::parameter_holder &transition_parameters) {
		if (is_transitionable() == false) {
			PE_LOG_ERROR("シーン遷移予約に失敗しました。遷移中のため、新たに遷移処理をはじめることはできません。");
			return false;
		} //遷移処理を始めることはできない
		//シーン遷移要求
		state_ = state::transition_requested;
		transition_parameters_ = transition_parameters;
		PE_LOG_MESSAGE("シーン遷移予約を行いました。");
		return true;
	}

	void standard_scene_manager::transition_proc() {
		//現在のシーンを終了
		const util::parameter_holder next_scene_initialize_parameters = end_current_scene();
		//新しいシーンを作成
		auto new_scene = std::make_shared<scene>();
		//新しいシーンを初期化
		if (!initialize_scene(*new_scene, *next_scene_setupper_, next_scene_initialize_parameters)) {
			PE_LOG_ERROR("シーン遷移に失敗しました。新しいシーン(", next_scene_id_, ")のセットアップまたは初期化に失敗しました。");
			transition_to_error_scene();
			return;
		}
		current_scene_ = std::move(new_scene);
		current_scene_setupper_ = std::move(next_scene_setupper_);
		//未使用リソースを削除(シーンの更新前のため、ここで削除してよい)
		assert(resource_manager_ != nullptr);
		resource_manager_->unload_unused_resources();
		//リクエストと準備状況をリセット
		state_ = state::progress;
		is_next_scene_loaded_ = false;
		PE_LOG_MESSAGE("シーン(", next_scene_id_, ")に遷移しました。");
		current_scene_id_ = next_scene_id_;
		next_scene_id_ = "";
	}

	bool standard_scene_manager::initialize() {
		current_scene_.reset();
		current_scene_setupper_.reset();
		state_ = state::none;
		is_next_scene_loaded_ = false;
		create_debug_information_channel("SceneManager");
		return true;
	}

	bool standard_scene_manager::finalize() {
		delete_debug_information_channel();
		//現在のシーンを終了
		end_current_scene();
		state_ = state::none;
		return true;
	}

	std::shared_ptr<scene_setupper> standard_scene_manager::create_scene_set_upper(const std::string &scene_name) {
		//シーン名にプレフィックスをつけたクラスを作成。
		auto setupper = reflection::reflection::create_object_by_object_type_id<scene_setupper>(
			add_prefix(scene_name, object_category::scene));
		return setupper;
	}

	void standard_scene_manager::transition_to_error_scene() {
		PE_LOG_ERROR("エラーシーンに遷移します。");
		const std::shared_ptr<scene_setupper> ecd = std::make_shared<s_error>();
		auto es = std::make_shared<scene>();
		initialize_scene(*es, *ecd, util::parameter_holder());
		current_scene_ = std::move(es);
		current_scene_setupper_ = ecd;
		state_ = state::progress;
		is_next_scene_loaded_ = false;
	}

	util::parameter_holder standard_scene_manager::end_current_scene() const {
		//現在のシーンを終了(現在のシーンがなかったら空のパラメータを格納)
		if (current_scene_) {
			return finalize_scene(*current_scene_, *current_scene_setupper_, next_scene_id_,
			                      transition_parameters_);
		}
		return {};
	}

	void standard_scene_manager::set_resource_manager(const std::shared_ptr<resource_manager> &mgr) {
		resource_manager_ = mgr;
	}

	void standard_scene_manager::debug_information_add_handler(i_debug_information_adder &di_adder) {
		di_adder.add_line_v("現在のシーンID:", current_scene_id_);
		if (current_scene_) { current_scene_->debug_information_add_handle(di_adder); }
	}

	bool standard_scene_manager::initialize_scene(scene &scene, scene_setupper &setupper,
	                                            const util::parameter_holder &init_param) {
		//標準のシーンModuleをセット
		set_standard_scene_modules(scene);
		//シーンModuleにシーンを登録
		scene.set_scene_to_modules();
		//システム設定(特殊プロセスの作成やシーンデータの更新)
		if (!standard_system_set_up_scene(scene)) {
			PE_LOG_ERROR("シーンのシステム設定に失敗しました。");
			return false;
		}
		//初期化
		if (!scene.initialize()) {
			PE_LOG_ERROR("シーンの初期化に失敗しました。");
			return false;
		}
		//固有設定
		if (!setupper.initialize_scene(scene, init_param)) {
			PE_LOG_ERROR("シーンの固有設定に失敗しました。");
			return false;
		}
		return true;
	}

	util::parameter_holder standard_scene_manager::finalize_scene(scene &scene, scene_setupper &setupper,
	                                                              const std::string &next_scene_id,
	                                                              const util::parameter_holder &finalize_parameters) {
		auto ret = setupper.terminate_scene(scene, next_scene_id, finalize_parameters); //固有終了処理
		scene.finalize(); //終了処理
		return ret;
	}
}
