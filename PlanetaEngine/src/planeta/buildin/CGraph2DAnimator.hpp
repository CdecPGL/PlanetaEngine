#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>

#include "planeta/GameObjectStandardComponent.hpp"
#include "planeta/core/WeakPointer.hpp"
#include "planeta/core/RectAngle.hpp"

namespace plnt {
	class CDrawGraph2D;
	/*! 平面画像アニメータコンポーネント*/
	class CGraph2DAnimator : public GameObjectStandardComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CGraph2DAnimator);
	public:
		using Super = GameObjectStandardComponent;
		/*! フレームデータのタイプ<表示フレーム数,表示範囲,拡大度,回転度(ラジアン)>*/
		using FrameDataType = std::tuple<unsigned int, RectAngle<int>, Vector2Dd, double>;
		/*! 単純化フレームデータのタイプ<表示フレーム数,表示範囲>*/
		using SimpleFrameDataType = std::pair<unsigned int, RectAngle<int>>;
		/*! アニメーションセット(フル)*/
		void SetAnimation(const std::string& anim_name, const std::vector<FrameDataType>& frames);
		/*! アニメーションセット(シンプル)*/
		void SetAnimation(const std::string& anim_name, const std::vector<SimpleFrameDataType>& frames);
		/*! 指定名のアニメーションの再生を開始する*/
		bool StartAnimation(const std::string& anim_name, bool roop_flag);
		/*! アニメーションが再生中なら停止する*/
		void StopAnimation();
		/*! 再生中のアニメーション名を取得する*/
		const std::string GetPlayingAnimationName()const { return is_playing_ ? current_animation_name_ : ""; }

		//アクセサ
		/*! 全アニメーションデータの設定*/
		void animation_data(const std::unordered_map<std::string,std::vector<FrameDataType>>& data);
	private:
		NonOwingPointer<CDrawGraph2D> draw_graph_component_;
		unsigned int frame_counter_ = 0;
		bool is_playing_ = false;
		bool roop_flag_ = false;
		unsigned int current_frame_ = 0;
		std::unordered_map<std::string, std::vector<FrameDataType>> animations_;
		std::vector<FrameDataType>* current_animation_ = nullptr;
		std::string current_animation_name_;
		void SetCurrentFrameDrawAreaToDrawGraphComponent_();
		bool GetOtherComponentsProc(const GOComponentGetter& com_getter) override;
		void OnInitialized()override;
		void UpdateAnimation();
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CGraph2DAnimator);
}