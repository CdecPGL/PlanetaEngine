#pragma once

#include <memory>

namespace planeta_engine {
	namespace debug {
		class DebuggerCommunicater final{
		public:
			DebuggerCommunicater();
			~DebuggerCommunicater();
			bool Initialize();
			void Finalize();
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
		};
	}
}
