#pragma once
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		/*Œø‰Ê‰¹ƒŠƒ\[ƒX*/
		class SoundResource : public core::ResourceBase {
		public:
			SoundResource() :_handle(-1) {};
			~SoundResource() {};
			int GetHandle()const { return _handle; }
			int GetTotalTimeByMilliSecond();
		private:
			int _handle;
			virtual bool _Create(const std::shared_ptr<const file_system::File>& file) override;
			virtual void _Dispose() override;
		};
	}
}
