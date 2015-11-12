#pragma once
#include "ResourceBase.h"

namespace planeta_engine {
	namespace resources {
		class TextResource : public core::ResourceBase {
		public:
			TextResource() {};
			~TextResource() {};
			const std::string& GetText()const { return _text; }
		private:
			virtual bool _Create(const std::shared_ptr<const file_system::File>& file) override;
			virtual void _Dispose() override;
			std::string _text;
		};
	}
}
