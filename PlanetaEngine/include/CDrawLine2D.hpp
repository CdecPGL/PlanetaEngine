#pragma once
#include "CDraw2D.hpp"
#include "Vector2D.hpp"
#include <array>
#include <vector>

namespace planeta {
	/*! 平面上直線描画コンポーネント*/
	class CDrawLine2D final : public CDraw2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawLine2D);
	public:
		using Super = CDraw2D;
		CDrawLine2D();
		/*アクセサ*/
		/*! 線の幅を取得*/
		int width()const { return _width; }
		/*! 線の幅を設定*/
		CDrawLine2D& width(int w) { _width = w; return *this; }
		/*! 線の長さを取得*/
		double length()const { return _length; }
		/*! 線の長さを設定*/
		CDrawLine2D& length(double l) { _length = l; return *this; }
	private:
		std::vector<Vector2Df> _wire_positions;
		int _width;
		double _length;
		/*描画処理*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawLine2D);
}