#pragma once
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		/*効果音リソース*/
		class SoundResource : public core::ResourceBase {
		public:
			SoundResource() :_handle(-1) {};
			~SoundResource() {};
			int GetHandle()const { return _handle; }
		private:
			int _handle;
			virtual bool _Create(const std::shared_ptr<file_system::File>& file) override;
			virtual void _Dispose() override;
		};
	}
}
