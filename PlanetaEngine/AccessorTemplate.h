#pragma once

namespace planeta_engine {
	namespace utility {
		template<class PublicInterface, class... SAImpls>
		class AccessorTemplate : public SAImpls...{
		public:
			explicit AccessorTemplate(PublicInterface& pi) :SAImpls(pi)...{}
		};
	}
}