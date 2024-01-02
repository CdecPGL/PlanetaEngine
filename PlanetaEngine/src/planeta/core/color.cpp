#include "color.hpp"

namespace plnt {
	const color &color::red() {
		static color color(255, 0, 0);
		return color;
	}

	const color &color::green() {
		static color color(0, 255, 0);
		return color;
	}

	const color &color::blue() {
		static color color(0, 0, 255);
		return color;
	}

	const color &color::yellow() {
		static color color(255, 255, 0);
		return color;
	}

	const color &color::cyan() {
		static color color(0, 255, 255);
		return color;
	}

	const color &color::magenta() {
		static color color(255, 0, 255);
		return color;
	}

	const color &color::white() {
		static color color(255, 255, 255);
		return color;
	}

	const color &color::black() {
		static color color(0, 0, 0);
		return color;
	}
}
