#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*効果音リソース*/
	class SoundResource : public core::ResourceBase {
	public:
		SoundResource() :_handle(-1) {};
		~SoundResource() {};
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const std::shared_ptr<const File>& file) override;
		virtual void _Dispose() override;
	};
}