#include "TextPanel.h"
#include "ResourceManager.h"
#include "RFontDefinition.h"
#include "ScreenDrawerGUI.h"

namespace planeta {
	/*namespace gui_components {

		void TextPanel::UpdateProc()
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		void TextPanel::DrawProc(const utility::RectAngle<int>& draw_area, ScreenDrawerGUI& drawer)
		{
			Vector2Dd dp = draw_area.position;
			for (auto& str : lines_) {
				drawer.DrawString(dp, character_scale_, str, color_, outline_color_, font_definition_resource_);
				dp.y += character_size_;
			}
		}

		bool TextPanel::SetFontDefinitionResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("リソースの取得に失敗しました。(リソース名は") + resource_id + ")", __FUNCTION__);
				return false;
			}
			std::shared_ptr<resources::FontDefinitionResource> fdr = std::dynamic_pointer_cast<resources::FontDefinitionResource>(res);
			if (fdr) {
				font_definition_resource_ = fdr;
				UpdateCharacterScale();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("フォント定義リソースでないリソースが指定されました。(リソース名は") + resource_id + "、タイプは" + res->GetType().name() + ")", __FUNCTION__);
				return false;
			}
		}

		void TextPanel::UpdateCharacterScale()
		{
			if (font_definition_resource_) {
				double s = (double)character_size() / font_definition_resource_->font_size();
				character_scale_.Set(s, s);
			}
		}

	}*/
}