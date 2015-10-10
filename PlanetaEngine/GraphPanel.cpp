#include "GraphPanel.h"

#include "ResourceManager.h"
#include "SystemLog.h"
#include "DrawManager.h"
#include "GraphResource.h"

namespace planeta_engine {
	namespace ui_objects {

		void GraphPanel::UpdateProc()
		{
			
		}

		void GraphPanel::DrawProc(const utility::RectAngle<int>& draw_area)
		{
			core::DrawManager::instance().DrawUIGraph(draw_area, this->draw_area(), reverse(), graph_resource_);
		}

		bool GraphPanel::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�X����") + resource_id + ")", __FUNCTION__);
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				graph_resource_ = gr;
				draw_area_.Set(Vector2D<int>(0, 0), Vector2D<int>(gr->size().x, gr->size().y));
				size(draw_area_.size);
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("�摜���\�[�X�łȂ����\�[�X���w�肳��܂����B(���\�[�X����") + resource_id + "�A�^�C�v��" + res->GetType().name() + ")", __FUNCTION__);
				return false;
			}
		}

	}
}