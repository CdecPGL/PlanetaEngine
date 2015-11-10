#pragma once

#include "FileSaverBase.h"

namespace planeta_engine {
	namespace file_system {
		class NormalFileSaver final: public FileSaverBase {
		public:
			FileSaverBase::FileSaverBase;
		private:
			bool SaveFile(const std::string& name, const std::shared_ptr<File>& file) override;
			bool _Initialize() override;
			void _Finalize() override;
		};
	}
}
