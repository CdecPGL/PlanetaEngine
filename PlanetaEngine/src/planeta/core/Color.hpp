#pragma once

#include "planeta/reflection/reflectable.hpp"

namespace plnt {
	class color : public reflection::reflectable {
		PE_REFLECTION_DATA_REGISTERER_DECL_DEF(color) {
			registerer
				.PE_REFLECTABLE_CLASS_PROPERTY(color, r)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, g)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, b)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, a)
				.shallow_copy_target(&color::_r)
				.shallow_copy_target(&color::_g)
				.shallow_copy_target(&color::_b)
				.shallow_copy_target(&color::_a);
		}

	public:
		color(int r, int g, int b, int a) : _r(r), _g(g), _b(b), _a(a) {
			if (_r < 0) { _r = 0; } else if (_r > 255) { _r = 255; }
			if (_g < 0) { _g = 0; } else if (_g > 255) { _g = 255; }
			if (_b < 0) { _b = 0; } else if (_b > 255) { _b = 255; }
			if (_a < 0) { _a = 0; } else if (_a > 255) { _a = 255; }
		}

		color(int r, int g, int b) : color(r, g, b, 255) { }

		color() : color(255, 255, 255) { }

		/*アクセサ*/
		int r() const { return _r; }
		void r(int pr) { _r = pr; }
		int g() const { return _g; }
		void g(int pg) { _g = pg; }
		int b() const { return _b; }
		void b(int pb) { _b = pb; }
		int a() const { return _a; }
		void a(int pa) { _a = pa; }

		const color operator+(const color &c) const { return color(_r + c._r, _g + c._g, _b + c._b, _a + c._a); }

		const color operator-(const color &c) const { return color(_r - c._r, _g - c._g, _b - c._b, _a - c._a); }

		static const color &Red();
		static const color &Green();
		static const color &Blue();
		static const color &Yellow();
		static const color &Cyan();
		static const color &Magenta();
		static const color &White();
		static const color &Black();

	private:
		int _r;
		int _g;
		int _b;
		int _a;
	};

	PE_REFLECTABLE_CLASS(color);
}
