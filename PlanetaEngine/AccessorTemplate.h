#pragma once

namespace planeta {
	namespace util {
		template<class PublicInterface, class... SAImpls>
		class AccessorTemplate : public SAImpls...{
		public:
			explicit AccessorTemplate(PublicInterface& pi) :SAImpls(pi)...{}
		};
	}
}