#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*音楽リソース*/
	class RMusic : public ResourceBase {
	public:
		RMusic() :_handle(-1) {};
		~RMusic() {}
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const File& file, ResourceReferencer&) override;
		virtual void _Dispose() override;
	};
}

