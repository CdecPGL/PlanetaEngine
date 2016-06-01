#include <stdexcept>

namespace planeta_engine{
	namespace util{
		class BadNewDeleteOperation : public std::logic_error{
		public:
			BadNewDeleteOperation(const std::string& e) :std::logic_error(e){}
		};
	}
}