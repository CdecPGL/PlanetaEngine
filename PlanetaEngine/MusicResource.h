#pragma once
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		/*音楽リソース*/
		class MusicResource : public core::ResourceBase {
		public:
			MusicResource() :_handle(-1) {};
			~MusicResource() {}
			int GetHandle()const { return _handle; }
			int GetTotalTimeByMilliSecond();
		private:
			int _handle;
			virtual bool _Create(const std::shared_ptr<file_system::File>& file) override;
			virtual void _Dispose() override;
		};
	}
}

