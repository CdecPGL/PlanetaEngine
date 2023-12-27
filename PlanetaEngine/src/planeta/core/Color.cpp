#include "Color.hpp"

namespace plnt {
	const color &color::Red() {
		static color color(255, 0, 0);
		return color;
	}

	const color &color::Green() {
		static color color(0, 255, 0);
		return color;
	}

	const color &color::Blue() {
		static color color(0, 0, 255);
		return color;
	}

	const color &color::Yellow() {
		static color color(255, 255, 0);
		return color;
	}

	const color &color::Cyan() {
		static color color(0, 255, 255);
		return color;
	}

	const color &color::Magenta() {
		static color color(255, 0, 255);
		return color;
	}

	const color &color::White() {
		static color color(255, 255, 255);
		return color;
	}

	const color &color::Black() {
		static color color(0, 0, 0);
		return color;
	}
}
