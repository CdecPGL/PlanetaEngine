#pragma once

#include <memory>
#include <vector>
#include "GUIComponent.h"
#include "Color.h"

namespace planeta_engine {
	namespace resources {
		class FontDefinitionResource;
	}
	namespace ui_components {
		/*複数行のテキストパネル*/
		class TextPanel : public game::GUIComponent {
		public:
			bool SetFontDefinitionResource(const std::string& resource_id);
			/*アクセサ*/
			const std::string& text()const { return text_; }
			void text(const std::string& t) { text_ = t; }
			const Color& color()const { return color_; }
			void color(const Color& c) { color_ = c; }
			const Color& outline_color()const { return outline_color_; }
			void outline_color(const Color& c) { outline_color_ = c; }
			const bool auto_newline_flag()const { return auto_newline_; }
			void auto_newline_flag(bool f) { auto_newline_ = f; }
			const int character_size()const { return character_size_; }
			void character_size(int s) { character_size_ = s; UpdateCharacterScale(); }
		private:
			std::string text_;
			std::vector<std::string> lines_;
			std::shared_ptr<resources::FontDefinitionResource> font_definition_resource_;
			Color color_;
			Color outline_color_;
			bool auto_newline_ = false;
			int character_size_ = 16;
			Vector2D<double> character_scale_;
			void UpdateCharacterScale();

			virtual void UpdateProc() override;
			virtual void DrawProc(const utility::RectAngle<int>& draw_area) override;
		};
	}
}
