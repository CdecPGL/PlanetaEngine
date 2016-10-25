#pragma once

#include <memory>
#include "DebugManager.h"

namespace planeta {
	namespace  private_ {
		class StandardDebugManager final: public DebugManager{
		public:
			StandardDebugManager();
			~StandardDebugManager();
			bool Initialize()override;
			void Finalize()override;
			void Update()override;
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}