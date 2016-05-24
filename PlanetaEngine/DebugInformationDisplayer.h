#pragma once
#include <memory>
#include "SingletonTemplate.h"
#include "StringUtility.h"

namespace planeta_engine {
	namespace  debug {
		class DebugInformationDisplayer : public utility::SingletonTemplate<DebugInformationDisplayer>{
			friend utility::SingletonTemplate<DebugInformationDisplayer>;
		public:
			DebugInformationDisplayer();
			~DebugInformationDisplayer();
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