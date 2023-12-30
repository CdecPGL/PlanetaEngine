#pragma once

#include <memory>
#include <vector>
#include <array>

#include "planeta/core/RectAngle.hpp"
#include "planeta/core/Vertex2D.hpp"

#include "CDraw2D.hpp"

namespace plnt {
	namespace private_ {
		class graph_draw_data_2d;
	}

	/*! 平面画像描画コンポーネント*/
	class CDrawGraph2D final : public CDraw2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGraph2D);

	public:
		using super = CDraw2D;
		CDrawGraph2D();
		~CDrawGraph2D();

		/*! 画像リソースをセット*/
		bool SetGraphResource(const std::string &resource_id);
		/*アクセサ*/
		/*! 画像リソースをIDでセット*/
		CDrawGraph2D &graph_resource_id(const std::string &res_id);
		/*! 画像上の描画領域をピクセル単位で取得*/
		const rect_angle<int> &draw_area() const { return _draw_area; }
		/*! 画像上の描画領域をピクセル単位で設定*/
		CDrawGraph2D &draw_area(const rect_angle<int> &rect) {
			_draw_area = rect;
			_UpdateUVPosition();
			return *this;
		}

		/*! 画像を反転させるか取得*/
		const bool reverse() const { return reverse_; }
		/*! 画像を反転させるか設定*/
		CDrawGraph2D &reverse(bool rev) {
			reverse_ = rev;
			_UpdateUVPosition();
			return *this;
		}

		/*! 画像の中心を[0.0,1.0]*[0.0,1.0]の範囲で取得*/
		const Vector2Dd &graph_center() const { return graph_center_; }
		/*! 画像の中心を[0.0,1.0]*[0.0,1.0]の範囲で設定*/
		void graph_center(const Vector2Dd &pos) { graph_center_ = pos; }

	private:
		/*反転描画フラグ*/
		bool reverse_ = false;
		/*画像上で表示する範囲*/
		rect_angle<int> _draw_area;
		/*画像の中心位置*/
		Vector2Dd graph_center_ = Vector2Dd(0.5, 0.5);
		/*画像描画データ*/
		std::shared_ptr<private_::graph_draw_data_2d> graph_draw_data_;
		/*ポリゴン情報更新*/
		void _UpdatePolygon();
		/*UV座標更新*/
		void _UpdateUVPosition();
		/*描画処理*/
		void DrawProc(ScreenDrawer2D &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGraph2D);
}
