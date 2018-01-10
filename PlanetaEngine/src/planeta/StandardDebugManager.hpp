#pragma once

#include <memory>
#include "DebugManager.hpp"

namespace plnt {
	namespace  private_ {
		/*標準のデバッグマネージャ*/
		class StandardDebugManager final: public DebugManager{
		public:
			StandardDebugManager();
			~StandardDebugManager()override;
			bool Initialize(RenderingManager& rendering_mgr)override;
			void Finalize()override;
			void PreRenderingUpdate()override;
			void PostRenderingUpdate()override;
			bool CreateDebugInformationChannel(const std::string& channel_id, const std::function<void(IDebugInformationAdder &) >& handler) override;
			bool DeleteDebugInformationChannel(const std::string& channel_id) override;
			bool CreateDebugDrawChannel(const std::string& channel_id, const std::function<void(IDebugDrawer &) > handler) override;
			bool DeleteDebugDrawChannel(const std::string& channel_id) override;
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}