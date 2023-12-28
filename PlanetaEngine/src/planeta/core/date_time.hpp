#pragma once

#include <ctime>

#include "Object.hpp"

namespace plnt::util {
	/*日にち時間クラス
		演算結果が負になる場合はすべて0とする*/
	class date_time final : public Object {
	public:
		date_time(): date_time(0, 0, 0, 0, 0, 0) { }

		/*コンストラクタ(年、月、日、時間、分、秒)*/
		date_time(const size_t y, const size_t mon, const size_t d, const size_t h, const size_t m, const size_t s) :
			second_(static_cast<signed>(s)), minute_(static_cast<signed>(m)), hour_(static_cast<signed>(h)),
			day_(static_cast<signed>(d)), month_(static_cast<signed>(mon)), year_(static_cast<signed>(y)) { }

		explicit date_time(const std::tm &c_time) : second_(c_time.tm_sec), minute_(c_time.tm_min),
		                                            hour_(c_time.tm_hour),
		                                            day_(c_time.tm_mday), month_(c_time.tm_mon + 1),
		                                            year_(c_time.tm_year + 1900) { }

		date_time(const date_time &t) = default;
		date_time(date_time &&t) = default;
		~date_time() override = default;
		date_time &operator=(const date_time &t) = default;
		date_time &operator=(date_time &&t) = default;

		/*時間のセット(時間、分、秒)*/
		void set_time(const size_t h, const size_t m, const size_t s) {
			second_ = static_cast<signed>(s);
			minute_ = static_cast<signed>(m);
			hour_ = static_cast<signed>(h);
		}

		//日付のセット(年、月、日)*/
		void set_date(const size_t y, const size_t m, const size_t d) {
			day_ = static_cast<signed>(d);
			month_ = static_cast<signed>(m);
			year_ = static_cast<signed>(y);
		}

		void reset() {
			set_time(0, 0, 0);
			set_date(0, 0, 0);
		}

		//アクセサ
		void second(const size_t s) { second_ = static_cast<signed>(s) % 60; }
		[[nodiscard]] int second() const { return second_; }
		void minute(const size_t m) { minute_ = static_cast<signed>(m) % 60; }
		[[nodiscard]] int minute() const { return minute_; }
		void hour(const size_t h) { second_ = static_cast<signed>(h); }
		[[nodiscard]] int hour() const { return hour_; }
		void day(const size_t d) { day_ = static_cast<signed>(d); }
		[[nodiscard]] int day() const { return day_; }
		void month(const size_t m) { month_ = static_cast<signed>(m); }
		[[nodiscard]] int month() const { return month_; }
		void year(const size_t y) { year_ = static_cast<signed>(y); }
		[[nodiscard]] int year() const { return year_; }
		////演算子
		//const DateTime& operator+=(const DateTime& t) {
		//	_second += t._second; _minute += t._minute; _hour += t._hour;
		//	_minute += _second / 60; _second %= 60;
		//	_hour += _minute / 60; _minute %= 60;
		//	return *this;
		//}
		//const DateTime& operator-=(const DateTime& t) {
		//	_second -= t._second; _minute -= t._minute; _hour -= t._hour;
		//	if (_second < 0) { --_minute; _second += 60; }
		//	if (_minute < 0) { --_hour; _minute += 60; }
		//	if (_hour < 0) { SetTime(0, 0, 0); }
		//	return *this;
		//}
		//const DateTime& operator*=(size_t n) {
		//	_second *= n; _minute *= n; _hour *= n;
		//	_minute += _second / 60; _second %= 60;
		//	_hour += _minute / 60; _minute %= 60;
		//	return *this;
		//}
		//const DateTime operator+(const DateTime& t)const {
		//	DateTime out(*this);
		//	return out += t;
		//}
		//const DateTime operator-(const DateTime& t)const {
		//	DateTime out(*this);
		//	return out -= t;
		//}
		//const DateTime operator*(int n)const {
		//	DateTime out(*this);
		//	return out *= n;
		//}
		bool operator==(const date_time &t) const {
			return second_ == t.second_ && minute_ == t.minute_ && hour_ == t.hour_ && day_ == t.day_ && month_ == t.
				month_ && year_ == t.year_;
		}

		bool operator!=(const date_time &t) const { return !(*this == t); }
		bool operator<=(const date_time &t) const { return !(*this > t); }
		bool operator>=(const date_time &t) const { return *this > t || *this == t; }
		bool operator<(const date_time &t) const { return !(*this >= t); }

		bool operator>(const date_time &t) const {
			if (year_ > t.year_) { return true; }
			if (year_ == t.year_) {
				if (month_ > t.month_) { return true; }
				if (month_ == t.month_) {
					if (day_ > t.day_) { return true; }
					if (day_ == t.day_) {
						if (hour_ > t.hour_) { return true; }
						if (hour_ == t.hour_) {
							if (minute_ > t.minute_) { return true; }
							if (minute_ == t.minute_) { return second_ > t.second_; }
							return false;
						}
						return false;
					}
					return false;
				}
				return false;
			}
			return false;
		}

		[[nodiscard]] std::string ToString() const override;

		/*現在時刻を取得*/
		static date_time get_current_date_time();

	private:
		int second_;
		int minute_;
		int hour_;
		int day_;
		int month_;
		int year_;
	};
}
