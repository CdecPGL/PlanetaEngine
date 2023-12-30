#pragma once

#include <ctime>

#include "object.hpp"

namespace plnt::util {
	/*日にち時間クラス
		演算結果が負になる場合はすべて0とする*/
	class date_time final : public object {
	public:
		date_time(): date_time(0, 0, 0, 0, 0, 0) { }

		/*コンストラクタ(年、月、日、時間、分、秒)*/
		date_time(const size_t y, const size_t mon, const size_t d, const size_t h, const size_t m, const size_t s) :
			year_(static_cast<signed>(y)), month_(static_cast<signed>(mon)), day_(static_cast<signed>(d)),
			hour_(static_cast<signed>(h)), minute_(static_cast<signed>(m)), second_(static_cast<signed>(s)) { }

		explicit date_time(const std::tm &c_time) : year_(c_time.tm_year + 1900), month_(c_time.tm_mon + 1),
		                                            day_(c_time.tm_mday),
		                                            hour_(c_time.tm_hour), minute_(c_time.tm_min),
		                                            second_(c_time.tm_sec) { }

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

		auto operator<=>(const date_time &t) const = default;

		[[nodiscard]] std::string to_string() const override;

		/*現在時刻を取得*/
		static date_time get_current_date_time();

	private:
		int year_;
		int month_;
		int day_;
		int hour_;
		int minute_;
		int second_;
	};
}
