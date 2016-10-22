#pragma once
#include "ResourceBase.h"

namespace planeta {
	/*効果音リソース*/
	class RSound : public private_::ResourceBase {
	public:
		RSound() :_handle(-1) {};
		~RSound() {};
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool _Create(const File& file, private_::ResourceReferencer&) override;
		virtual void _Dispose() override;
	};
}