#pragma once

#include <unordered_map>
#include <memory>
#include <bitset>
#include <array>
#include "planeta/Object.hpp"
#include "planeta/core/RectAngle.hpp"
#include "ObjectHolderTemplate_WithoutID.hpp"
#include "NonCopyable.hpp"

namespace plnt {
	//class ScreenDrawerGUI;
	//namespace game {
	//	class GUIComponent : public core::Object,private utility::NonCopyable<GUIComponent>{
	//	public:
	//		GUIComponent();
	//		virtual ~GUIComponent() = default;
	//		/*システム関数*/
	//		struct UpdateInfo {
	//			bool is_position_updated = false;
	//			bool is_size_updated = false;
	//			bool is_edge_fix_updated = false;
	//		};
	//		struct DrawData {
	//			utility::RectAngle<int> draw_area;
	//			UpdateInfo update_info;
	//		};
	//		bool Initialize();
	//		void Finalize();
	//		void Update();
	//		void Draw(const DrawData& parent_draw_data, ScreenDrawerGUI& drawer);
	//		void DebugDraw(const utility::RectAngle<int>& parent_draw_area);

	//		enum class edge { Right = 0, Left = 1, Top = 2, Bottom = 3 };
	//		/*公開関数*/
	//		/**
	//		* @brief 辺を固定する
	//		* @param (e) 対象の辺
	//		* @param (padding) 余白
	//		*/
	//		void FixEdge(edge e, int padding);
	//		/**
	//		* @brief すべての辺を固定する
	//		* @param (left_padding) 左辺の余白
	//		* @param (right_padding) 左辺の余白
	//		* @param (top_padding) 左辺の余白
	//		* @param (bottom_padding) 左辺の余白
	//		*/
	//		void FixAllEdge(int left_padding, int right_padding, int top_padding, int bottom_padding);
	//		/**
	//		* @brief 辺の固定を解除
	//		* @param (e) 対象の辺
	//		*/
	//		void UnfixEdge(edge e);
	//		/*アクセサ*/
	//		const utility::RectAngle<int>& rect()const { return rect_angle_; }
	//		void rect(const utility::RectAngle<int>& r) { rect_angle_ = r; is_position_updated_since_last_draw = true; is_size_updated_since_last_draw = true; }
	//		const Vector2Di position()const { return rect_angle_.position; }
	//		void position(const Vector2Di& p) { rect_angle_.position = p; is_position_updated_since_last_draw = true; }
	//		const Vector2Di size()const { return rect_angle_.size; }
	//		void size(const Vector2Di& s) { rect_angle_.size = s; is_size_updated_since_last_draw = true; }

	//		template<class C>
	//		std::shared_ptr<C> AddChild() {
	//			auto child = std::make_shared<C>();
	//			children_holder_->resist_object(child);
	//			return child;
	//		}
	//		bool RemoveChild(const std::shared_ptr<GUIComponent>& c) { return children_holder_->unresist_object(c); }
	//	private:

	//		std::unique_ptr<utility::ObjectHolderTemplate_WithoutID<GUIComponent>> children_holder_;
	//		bool is_position_updated_since_last_draw = true; //前回描画時から位置が更新されたかフラグ
	//		bool is_size_updated_since_last_draw = true; //前回描画時からサイズが変更されたかフラグ
	//		bool is_edge_fix_updated_since_last_draw = true; //前回描画時から辺固定が変更されたかフラグ
	//		utility::RectAngle<int> relative_draw_area_buffer_; //描画エリア
	//		void CalculateDrawRectAndUpdateInfo(const DrawData& parent_draw_data, UpdateInfo* my_update_info); //描画エリアと更新情報を計算する
	//		 /*位置情報*/
	//		utility::RectAngle<int> rect_angle_;
	//		/*アンカー*/
	//		std::array<int, 4> edge_fix_padding_; //辺固定の余白
	//		std::bitset<4> edge_fix_bit_set_ = 0; //辺固定の有無を表すビット配列

	//		/*子供処理*/
	//		void UpdateChildren();
	//		void DrawChildren(const DrawData& my_draw_data, ScreenDrawerGUI& drawer);

	//		/*オーバーライド可能関数群*/
	//		/*更新処理*/
	//		virtual void UpdateProc() {};
	//		/*前回描画時から位置が更新されたときの処理(描画前に呼ばれる)*/
	//		virtual void PositionUpdatedProc(const Vector2Di& position) {};
	//		/*前回描画時からサイズが更新されたときの処理(描画前に呼ばれる)*/
	//		virtual void SizeUpdatedProc(const Vector2Di& size) {};
	//		/*描画処理。描画は引数として渡される範囲内に行う*/
	//		virtual void DrawProc(const utility::RectAngle<int>& my_draw_area,ScreenDrawerGUI& drawer) = 0;
	//		/*初期化処理*/
	//		virtual bool InitializeProc() { return true; };
	//		/*終了処理*/
	//		virtual void FinalizeProc() {};
	//	};
	//}
}
