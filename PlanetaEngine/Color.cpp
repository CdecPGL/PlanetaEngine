#include "Color.h"

namespace planeta_engine {
	namespace core {

		const Color& Color::Red()
		{
			static Color color(255, 0, 0);
			return color;
		}

		const Color& Color::Green()
		{
			static Color color(0, 255, 0);
			return color;
		}

		const Color& Color::Blue()
		{
			static Color color(0, 0, 255);
			return color;
		}

		const Color& Color::Yellow()
		{
			static Color color(255, 255, 0);
			return color;
		}

		const Color& Color::Cyan()
		{
			static Color color(0, 255, 255);
			return color;
		}

		const Color& Color::Magenta()
		{
			static Color color(255, 0, 255);
			return color;
		}

		const Color& Color::White()
		{
			static Color color(255, 255, 255);
			return color;
		}

		const Color& Color::Black()
		{
			static Color color(0, 0, 0);
			return color;
		}

	}
}