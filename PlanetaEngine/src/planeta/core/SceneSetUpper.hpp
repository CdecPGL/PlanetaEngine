#pragma once

#include <vector>
#include <functional>

#include "object.hpp"
#include "parameter_holder.hpp"
#include "non_copyable.hpp"

namespace plnt {
	namespace private_ {
		class scene;
	}

	class scene_set_up_proxy;

	/*シーン設定クラス。オブジェクト引継ぎ処理は未実装*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class scene_setupper : public object, util::non_copyable<scene_setupper> {
	public:
		using super = object;
		scene_setupper() = default;
		scene_setupper(scene_setupper &&) = delete;
		~scene_setupper() override = default;
		scene_setupper &operator=(scene_setupper &&) = delete;

		/*シーンを初期化する
		@param 対象シーン、初期化パラメータリスト
		*/
		bool initialize_scene(private_::scene &scene, const util::parameter_holder &initialize_parameters);
		/*シーンを終了する
		@param 対象シーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		util::parameter_holder terminate_scene(private_::scene &scene, const std::string &next_scene_id,
		                                       const util::parameter_holder &finalize_parameters);

	private:
		/*シーンの設定処理を行う(初期オブジェクト、UI、プロセスの追加、[前シーンからオブジェクトの引継ぎ]未実装)
		@param 設定するシーン、初期化パラメータリスト
		*/
		virtual bool setup_scene(scene_set_up_proxy &scene_proxy, const util::parameter_holder &initialize_parameters) = 0;
		/*シーンの終了処理を行う(次シーンの初期化パラメータ生成など)
		@param 終了処理するシーン、次のシーンのID、終了処理パラメータリスト
		@return 次のシーンの初期化パラメータ
		*/
		virtual util::parameter_holder finalize_scene(scene_set_up_proxy &scene_proxy, const std::string &next_scene_id,
		                                              const util::parameter_holder &finalize_parameters) = 0;
	};

	PE_REFLECTABLE_CLASS(scene_setupper);
}

#define PE_SCENE_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
