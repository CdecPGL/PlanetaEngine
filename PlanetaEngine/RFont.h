#pragma once

#include <windef.h>
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta {
	/*�t�H���g���\�[�X*/
	class RFont final : public core::ResourceBase {
	public:
		RFont() {};
		~RFont() {};
		HANDLE GetHandle()const { return handle_; }
	private:
		HANDLE handle_ = 0;
		DWORD font_num_ = 0;
		bool _Create(const std::shared_ptr<const File>& file) override;
		void _Dispose() override;
	};
}