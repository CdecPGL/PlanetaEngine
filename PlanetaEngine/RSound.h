#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*効果音リソース*/
	class RSound : public ResourceBase {
	public:
		RSound() :_handle(-1) {};
		~RSound() {};
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const File& file, ResourceReferencer&) override;
		virtual void _Dispose() override;
	};
}