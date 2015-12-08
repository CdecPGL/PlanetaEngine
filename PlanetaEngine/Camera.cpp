#include "Camera.h"

#include "Vector3D.h"
#include "Property.h"

namespace planeta_engine {
	class test {
	public:
		test():x(*this) {}
	private:
		int x_;
		void set_x(int v) { x_ = v; }
		int get_x()const { return x_; }
	public:
		utility::Property<test, int, &test::get_x, &test::set_x> x;
	};
	namespace core {
		void Camera::ApplyToDxLib() {
			test t;
			t.x = 9;
			int i = t.x;
			++t.x;
			t.x++;
			--t.x;
			t.x--;
			t.x += 2;
			t.x -= 8;
			t.x *= 7;
			t.x /= 6;
			t.x ^= 4;
			t.x |= 34;
			t.x ^= 23;
			t.x >>= 8;
			t.x <<= 23;
			t.x();
			t.x(23);

			math::Vector3D<int> v3i;
		}
	}
}