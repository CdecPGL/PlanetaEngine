#include "GraphPanel.h"

#include "ResourceManager.h"
#include "SystemLog.h"
#include "DrawManager.h"
#include "GraphResource.h"

namespace planeta_engine {
	namespace ui_components {

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
				debug::SystemLog::instance().LogError(std::string("リソースの取得に失敗しました。(リソース名は") + resource_id + ")", __FUNCTION__);
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				graph_resource_ = gr;
				relative_draw_area_buffer_.Set(math::Vector2Di(0, 0), math::Vector2Di(gr->size().x, gr->size().y));
				size(relative_draw_area_buffer_.size);
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("画像リソースでないリソースが指定されました。(リソース名は") + resource_id + "、タイプは" + res->GetType().name() + ")", __FUNCTION__);
				return false;
			}
		}

	}
}