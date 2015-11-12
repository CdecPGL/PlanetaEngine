#pragma once

#include <windef.h>
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace resources {
		/*フォントリソース*/
		class FontResource final: public core::ResourceBase {
		public:
			FontResource() {};
			~FontResource() {};
			HANDLE GetHandle()const { return handle_; }
		private:
			HANDLE handle_ = 0;
			DWORD font_num_ = 0;
			bool _Create(const std::shared_ptr<const file_system::File>& file) override;
			void _Dispose() override;
		};
	}
}
