#pragma once
#include "planeta/GUIComponent.hpp"
#include "Color.hpp"

namespace plnt {
	//namespace resources {
	//	class FontDefinitionResource;
	//}
	//namespace gui_components {
	//	/*単行のテキストパネル*/
	//	class StringPanel : public game::GUIComponent {
	//	public:
	//		bool SetFontDefinitionResource(const std::string& resource_id);
	//		/*アクセサ*/
	//		const std::string& text()const { return text_; }
	//		void text(const std::string& t) { text_ = t; }
	//		const Color& color()const { return color_; }
	//		void color(const Color& c) { color_ = c; }
	//		const Color& outline_color()const { return outline_color_; }
	//		void outline_color(const Color& c) { outline_color_ = c; }
	//		const int character_size()const { return character_size_; }
	//		void character_size(int s) { character_size_ = s; UpdateCharacterScale(); }
	//	private:
	//		std::string text_;
	//		std::shared_ptr<resources::FontDefinitionResource> font_definition_resource_;
	//		Color color_;
	//		Color outline_color_;
	//		int character_size_ = 16;
	//		Vector2Dd character_scale_;
	//		void UpdateCharacterScale();

	//		virtual void UpdateProc() override;
	//		virtual void DrawProc(const utility::RectAngle<int>& draw_area,ScreenDrawerGUI& drawer) override;
	//		virtual bool InitializeProc()override;
	//	};
	//}
}

