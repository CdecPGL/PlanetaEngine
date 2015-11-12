#pragma once

#include "FileSaverBase.h"

namespace planeta_engine {
	namespace file_system {
		class NormalFileSaver final: public FileSaverBase {
		public:
			FileSaverBase::FileSaverBase;
		private:
			bool SaveFile(const std::string& name, const File& file) override;
			bool InitializeCore() override;
			void FinalizeCore() override;
		};
	}
}
