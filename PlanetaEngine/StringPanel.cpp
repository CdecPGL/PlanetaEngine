#include "StringPanel.h"
#include "ResourceManager.h"
#include "FontDefinitionResource.h"
#include "DrawManager.h"

namespace planeta_engine {
	namespace ui_components {
		void StringPanel::UpdateProc()
		{
			
		}

		void StringPanel::DrawProc(const utility::RectAngle<int>& draw_area)
		{
			math::Vector2Dd dp = draw_area.position;
			core::DrawManager::instance().DrawUIString(dp, character_scale_, text_, color_, outline_color_, font_definition_resource_);
		}

		bool StringPanel::SetFontDefinitionResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����") + resource_id + ")", __FUNCTION__);
				return false;
			}
			std::shared_ptr<resources::FontDefinitionResource> fdr = std::dynamic_pointer_cast<resources::FontDefinitionResource>(res);
			if (fdr) {
				font_definition_resource_ = fdr;
				UpdateCharacterScale();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�t�H���g��`���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����") + resource_id + "�A�^�C�v��" + res->GetType().name() + ")", __FUNCTION__);
				return false;
			}
		}

		void StringPanel::UpdateCharacterScale()
		{
			if (font_definition_resource_) {
				double s = (double)character_size() / font_definition_resource_->font_size();
				character_scale_.Set(s, s);
			}
		}

		bool StringPanel::InitializeProc()
		{
			UpdateCharacterScale();
			return true;
		}

	}
}