#pragma once

#include "CDrawGUI.h"
#include "RectAngle.h"

namespace planeta {
	/*! GUI領域に画像を描画するコンポーネント*/
	class CDrawGraphGUI final: public CDrawGUI {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGraphGUI);
	public:
		using Super = CDrawGUI;
		CDrawGraphGUI();
		~CDrawGraphGUI();
		/*! 表示する画像のリソースIDを設定する*/
		bool resource_id(const std::string& res_id);
		/*! 画像上の描画領域をピクセル単位で取得*/
		const RectAnglei& draw_area()const;
		/*! 画像上の描画領域をピクセル単位で設定*/
		CDrawGraphGUI& draw_area(const RectAnglei& rect);
		/*! 画像を反転させるか取得*/
		const bool reverse()const;
		/*! 画像を反転させるか設定*/
		CDrawGraphGUI& reverse(bool rev);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void DrawProc(ScreenDrawerGUI& drawer) override;
		bool GetOtherComponentsProc(const GOComponentGetter&) override;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGraphGUI);
}