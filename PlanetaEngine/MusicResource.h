#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*音楽リソース*/
	class MusicResource : public core::ResourceBase {
	public:
		MusicResource() :_handle(-1) {};
		~MusicResource() {}
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const std::shared_ptr<const File>& file) override;
		virtual void _Dispose() override;
	};
}

