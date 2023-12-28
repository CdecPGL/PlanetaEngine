#pragma once

#include "planeta/reflection/reflectable.hpp"

namespace plnt {
	class color final : public reflection::reflectable {
		PE_REFLECTION_DATA_REGISTERER_DECL_DEF(color) {
			registerer
				.PE_REFLECTABLE_CLASS_PROPERTY(color, r)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, g)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, b)
				.PE_REFLECTABLE_CLASS_PROPERTY(color, a)
				.shallow_copy_target(&color::r_)
				.shallow_copy_target(&color::g_)
				.shallow_copy_target(&color::b_)
				.shallow_copy_target(&color::a_);
		}

	public:
		color(const int r, const int g, const int b, const int a) : r_(r), g_(g), b_(b), a_(a) {
			if (r_ < 0) { r_ = 0; } else if (r_ > 255) { r_ = 255; }
			if (g_ < 0) { g_ = 0; } else if (g_ > 255) { g_ = 255; }
			if (b_ < 0) { b_ = 0; } else if (b_ > 255) { b_ = 255; }
			if (a_ < 0) { a_ = 0; } else if (a_ > 255) { a_ = 255; }
		}

		color(const int r, const int g, const int b) : color(r, g, b, 255) { }

		color() : color(255, 255, 255) { }

		/*アクセサ*/
		[[nodiscard]] int r() const { return r_; }
		void r(const int pr) { r_ = pr; }
		[[nodiscard]] int g() const { return g_; }
		void g(const int pg) { g_ = pg; }
		[[nodiscard]] int b() const { return b_; }
		void b(const int pb) { b_ = pb; }
		[[nodiscard]] int a() const { return a_; }
		void a(const int pa) { a_ = pa; }

		color operator+(const color &c) const { return {r_ + c.r_, g_ + c.g_, b_ + c.b_, a_ + c.a_}; }

		color operator-(const color &c) const { return {r_ - c.r_, g_ - c.g_, b_ - c.b_, a_ - c.a_}; }

		static const color &red();
		static const color &green();
		static const color &blue();
		static const color &yellow();
		static const color &cyan();
		static const color &magenta();
		static const color &white();
		static const color &black();

	private:
		int r_;
		int g_;
		int b_;
		int a_;
	};

	PE_REFLECTABLE_CLASS(color);
}
