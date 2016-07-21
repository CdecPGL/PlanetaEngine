#pragma once
#include <memory>
#include "SingletonTemplate.h"
#include "StringUtility.h"

namespace planeta {
	namespace  debug {
		class DebugManager : public util::SingletonTemplate<DebugManager>{
			friend util::SingletonTemplate<DebugManager>;
		public:
			DebugManager();
			~DebugManager();
			bool Initialize()override;
			void Finalize()override;
			void Update();
			template<typename... Details>
			void PushDebugInformation(Details&&... details) {
				util::ConvertAndConnectToString(std::forward<Details>(details)...);
			}
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}