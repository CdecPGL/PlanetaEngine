#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*���ʉ����\�[�X*/
	class RSound : public core::ResourceBase {
	public:
		RSound() :_handle(-1) {};
		~RSound() {};
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const std::shared_ptr<const File>& file) override;
		virtual void _Dispose() override;
	};
}