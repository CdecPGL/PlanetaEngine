#pragma once

#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		class FontDefinitionResource final: public core::ResourceBase {
		public:
			int GetHandle()const { return handle_; }
			int font_size()const { return size_; }
		private:
			bool _Create(const std::shared_ptr<file_system::File>& file) override;
			void _Dispose() override;
			int handle_ = -1;
			int size_ = 0;
		};
	}
}