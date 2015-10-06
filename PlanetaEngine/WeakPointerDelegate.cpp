#include "WeakPointerDelegate.h"

namespace planeta_engine {
	/*class C1 {
	public:
		void TestFunc(const int& t) {

		}
	};
	class C2 {
	public:
		void testTest(const int& u) {

		}
	};
	namespace utility {
		std::shared_ptr<C1> c1s = std::make_shared<C1>();
		std::shared_ptr<C2> c2s = std::make_shared<C2>();
		WeakPointer<C1> c1w = c1s;
		WeakPointer<C2> c2w = c2s;
		void test() {
			WeakPointerDelegate<int> del;
			auto connect1 = del.Add(c1w, &C1::TestFunc);
			del(2);
			auto connect2 = del.Add(c2w, &C2::testTest);
			del(3);
			connect1.Remove();
			del(10);
			del.Add([](const int&)->void {});
		}
	}*/
}