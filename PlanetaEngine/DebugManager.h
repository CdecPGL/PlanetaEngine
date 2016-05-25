#pragma once
#include <memory>
#include "SingletonTemplate.h"
#include "StringUtility.h"

namespace planeta_engine {
	namespace  debug {
		class DebugManager : public utility::SingletonTemplate<DebugManager>{
			friend utility::SingletonTemplate<DebugManager>;
		public:
			DebugManager();
			~DebugManager();
			bool Initialize()override;
			bool Finalize()override;
			void Update();
			template<typename... Details>
			void PushDebugInformation(Details&&... details) {
				utility::ConvertAndConnectToString(std::forward<Details>(details)...);
			}
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	};
}