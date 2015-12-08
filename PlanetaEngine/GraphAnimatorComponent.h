#pragma once

#include <unordered_map>
#include <vector>
#include <tuple>

#include "Component.h"
#include "WeakPointer.h"
#include "RectAngle.h"

namespace planeta_engine {
	namespace components {
		class DrawGraphComponent;
		class GraphAnimatorComponent : public game::Component {
		public:
			/*フレームデータのタイプ<表示フレーム数,表示範囲,拡大度,回転度(ラジアン)>*/
			using FrameDataType = std::tuple<unsigned int, utility::RectAngle<int>,Vector2D<double>,double>;
			/*単純化フレームデータのタイプ<表示フレーム数,表示範囲>*/
			using SimpleFrameDataType = std::pair<unsigned int, utility::RectAngle<int>>;
			/*アニメーションセット(フル)*/
			void SetAnimation(const std::string& anim_name,const std::vector<FrameDataType>& frames);
			/*アニメーションセット(シンプル)*/
			void SetAnimation(const std::string& anim_name, const std::vector<SimpleFrameDataType>& frames);
			/*指定名のアニメーションの再生を開始する*/
			bool StartAnimation(const std::string& anim_name, bool roop_flag);
			/*アニメーションが再生中なら停止する*/
			void StopAnimation();
			/*再生中のアニメーション名を取得する*/
			const std::string GetPlayingAnimationName()const { return is_playing_ ? current_animation_name_ : ""; }
		private:
			utility::WeakPointer<DrawGraphComponent> draw_graph_component_;
			unsigned int frame_counter_ = 0;
			bool is_playing_ = false;
			bool roop_flag_ = false;
			unsigned int current_frame_ = 0;
			std::unordered_map<std::string, std::shared_ptr<std::vector<FrameDataType>>> animations_;
			std::shared_ptr<std::vector<FrameDataType>> current_animation_;
			std::string current_animation_name_;
			void SetCurrentFrameDrawAreaToDrawGraphComponent_();
			bool Initialize_() override;
			void Update_() override;

		};
	}
}