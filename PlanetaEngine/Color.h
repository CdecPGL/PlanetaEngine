#pragma once

namespace planeta {
	class Color {
	public:
		Color(int r, int g, int b, int a) :_r(r), _g(g), _b(b), _a(a) {
			if (_r < 0) { _r = 0; } else if (_r>255) { _r = 255; }
			if (_g < 0) { _g = 0; } else if (_g>255) { _g = 255; }
			if (_b < 0) { _b = 0; } else if (_b>255) { _b = 255; }
			if (_a < 0) { _a = 0; } else if (_a>255) { _a = 255; }
		}
		Color(int r, int g, int b) :Color(r, g, b, 255) {}
		Color() :Color(255, 255, 255) {}
		/*アクセサ*/
		int r()const { return _r; }
		void r(int pr) { _r = pr; }
		int g()const { return _g; }
		void g(int pg) { _g = pg; }
		int b()const { return _b; }
		void b(int pb) { _b = pb; }
		int a()const { return _a; }
		void a(int pa) { _a = pa; }
		const Color operator+(const Color& c)const {
			return Color(_r + c._r, _g + c._g, _b + c._b, _a + c._a);
		}
		const Color operator-(const Color& c)const {
			return Color(_r - c._r, _g - c._g, _b - c._b, _a - c._a);
		}
		static const Color& Red();
		static const Color& Green();
		static const Color& Blue();
		static const Color& Yellow();
		static const Color& Cyan();
		static const Color& Magenta();
		static const Color& White();
		static const Color& Black();
	private:
		int _r;
		int _g;
		int _b;
		int _a;
	};
}