#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*���y���\�[�X*/
	class RMusic : public core::ResourceBase {
	public:
		RMusic() :_handle(-1) {};
		~RMusic() {}
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const std::shared_ptr<const File>& file) override;
		virtual void _Dispose() override;
	};
}

