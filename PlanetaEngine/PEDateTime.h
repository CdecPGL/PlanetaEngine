#pragma once

#include <ctime>
#include "Object.h"

namespace planeta_engine {
	namespace utility {
		/*���ɂ����ԃN���X
		���Z���ʂ����ɂȂ�ꍇ�͂��ׂ�0�Ƃ���*/
		class DateTime : public core::Object {
		public:
			DateTime():DateTime(0, 0, 0, 0, 0, 0) {}
			/*�R���X�g���N�^(�N�A���A���A���ԁA���A�b)*/
			DateTime(size_t y,size_t mon,size_t d,size_t h, size_t m, size_t s) :_second((signed)s), _minute((signed)m), _hour((signed)h),_day((signed)d),_month((signed)mon),_year((signed)y) {}
			DateTime(const std::tm& ctime) :_second(ctime.tm_sec), _minute(ctime.tm_min), _hour(ctime.tm_hour),_day(ctime.tm_mday),_month(ctime.tm_mon),_year(ctime.tm_year+1900) {}
			~DateTime() {};
			/*���Ԃ̃Z�b�g(���ԁA���A�b)*/
			void SetTime(size_t h, size_t m, size_t s) { _second = (signed)s; _minute = (signed)m; _hour = (signed)h; }
			//���t�̃Z�b�g(�N�A���A��)*/
			void SetDate(size_t y, size_t m, size_t d) { _day = (signed)d; _month = (signed)m; _year = (signed)y; }
			void Reset() { SetTime(0, 0, 0); SetDate(0, 0, 0); }
			//�A�N�Z�T
			void second(size_t s) { _second = (signed)s % 60; }
			int second()const { return (unsigned)_second; }
			void minute(size_t m) { _minute = (signed)m % 60; }
			int minute()const { return (unsigned)_minute; }
			void hour(size_t h) { _second = (signed)h; }
			int hour()const { return (unsigned)_hour; }
			void day(size_t d) { _day = (signed)d; }
			int day()const { return (unsigned)_day; }
			void month(size_t m) { _month = (signed)m; }
			int month()const { return (unsigned)_month; }
			void year(size_t y) { _year = (signed)y; }
			int year()const { return (unsigned)_year; }
			////���Z�q
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
			const bool operator==(const DateTime& t)const { return (_second == t._second) && (_minute == t._minute) && (_hour == t._hour) && (_day == t._day) && (_month == t._month) && (_year == t._year); }
			const bool operator!=(const DateTime& t)const { return !(*this == t); }
			const bool operator<=(const DateTime& t)const { return !(*this > t); }
			const bool operator>=(const DateTime& t)const { return (*this > t) || (*this == t); }
			const bool operator<(const DateTime& t)const { return !(*this >= t); }
			const bool operator>(const DateTime& t)const {
				if (_year > t._year) { return true; }
				else if (_year == t._year) {
					if (_month > t._month) { return true; }
					else if (_month == t._month) {
						if (_day > t._day) { return true; }
						else if (_day == t._day) {
							if (_hour > t._hour) { return true; }
							else if (_hour == t._hour) {
								if (_minute > t._minute) { return true; }
								else {
									if (_minute == t._minute) { return _second > t._second; }
									else { return false; }
								}
							}
							else { return false; }
						}
						else { return false; }
					}
					else { return false; }
				}
				else { return false; }
				
			}

			std::string ToString()const override;

			/*���ݎ������擾*/
			static DateTime GetCurrentDateTime();
		private:
			int _second;
			int _minute;
			int _hour;
			int _day;
			int _month;
			int _year;
		};

	}
}
