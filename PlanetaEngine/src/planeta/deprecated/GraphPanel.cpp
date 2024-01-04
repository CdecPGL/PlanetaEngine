#include "planeta/core/GraphPanel.hpp"

#include "..\core\standard_resource_manager.hpp"
#include "..\core\log_utility.hpp"
#include "..\core\screen_drawer_gui.hpp"
#include "..\buildin\r_graph.hpp"

namespace plnt {
	/*namespace gui_components {

		void GraphPanel::UpdateProc()
		{

		}

		void GraphPanel::DrawProc(const utility::RectAngle<int>& draw_area,ScreenDrawerGUI& drawer)
		{
			drawer.DrawGraph(draw_area, this->draw_area(), reverse(), graph_resource_);
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
				relative_draw_area_buffer_.Set(vector_2di(0, 0), vector_2di(gr->size().x, gr->size().y));
				size(relative_draw_area_buffer_.size);
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("画像リソースでないリソースが指定されました。(リソース名は") + resource_id + "、タイプは" + res->GetType().name() + ")", __FUNCTION__);
				return false;
			}
		}

	}*/
}
