#pragma once

#include <ctime>

#include "object.hpp"

namespace plnt::util {
	/*時間クラス
		演算結果が負になる場合はすべて0とする*/
	class time final : public object {
	public:
		time() : time(0) {}

		/*コンストラクタ(秒)*/
		explicit time(const size_t s) : time(0, s) {}

		/*コンストラクタ(分、秒)*/
		time(const size_t m, const size_t s) : time(0, m, s) {}

		/*コンストラクタ(時間、分、秒)*/
		time(const size_t h, const size_t m, const size_t s) : hour_(static_cast<signed>(h)),
		                                                       minute_(static_cast<signed>(m)),
		                                                       second_(static_cast<signed>(s)) {}

		explicit time(const std::tm &c_time) : hour_(c_time.tm_hour), minute_(c_time.tm_min), second_(c_time.tm_sec) {}

		/*時間のセット(時間、分、秒)*/
		void set(const size_t h, const size_t m, const size_t s) {
			second_ = static_cast<signed>(s);
			minute_ = static_cast<signed>(m);
			hour_ = static_cast<signed>(h);
		}

		void reset() { set(0, 0, 0); }

		//アクセサ
		void second(const size_t s) { second_ = static_cast<signed>(s) % 60; }
		[[nodiscard]] int second() const { return second_; }
		void minute(const size_t m) { minute_ = static_cast<signed>(m) % 60; }
		[[nodiscard]] int minute() const { return minute_; }
		void hour(const size_t h) { second_ = static_cast<signed>(h); }
		[[nodiscard]] int hour() const { return hour_; }

		//演算子
		const time &operator+=(const time &t) {
			second_ += t.second_;
			minute_ += t.minute_;
			hour_ += t.hour_;
			minute_ += second_ / 60;
			second_ %= 60;
			hour_ += minute_ / 60;
			minute_ %= 60;
			return *this;
		}

		const time &operator-=(const time &t) {
			second_ -= t.second_;
			minute_ -= t.minute_;
			hour_ -= t.hour_;
			if (second_ < 0) {
				--minute_;
				second_ += 60;
			}
			if (minute_ < 0) {
				--hour_;
				minute_ += 60;
			}
			if (hour_ < 0) { set(0, 0, 0); }
			return *this;
		}

		const time &operator*=(const uint32_t n) {
			second_ = static_cast<int>(second_ * n);
			minute_ = static_cast<int>(minute_ * n);
			hour_ = static_cast<int>(hour_ * n);
			minute_ += second_ / 60;
			second_ %= 60;
			hour_ += minute_ / 60;
			minute_ %= 60;
			return *this;
		}

		time operator+(const time &t) const {
			time out(*this);
			return out += t;
		}

		time operator-(const time &t) const {
			time out(*this);
			return out -= t;
		}

		time operator*(const int n) const {
			time out(*this);
			return out *= n;
		}

		auto operator<=>(const time &) const = default;

		[[nodiscard]] std::string to_string() const override;

		/*! 現在時刻を取得*/
		[[nodiscard]] static time get_current_time();

	private:
		int hour_;
		int minute_;
		int second_;
	};
}
