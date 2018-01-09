#pragma once

#include <ctime>
#include "Object.hpp"

namespace planeta {
	namespace util {
		/*時間クラス
		演算結果が負になる場合はすべて0とする*/
		class Time : public Object {
		public:
			Time() :Time(0) {}
			/*コンストラクタ(秒)*/
			Time(size_t s) :Time(0, s) {}
			/*コンストラクタ(分、秒)*/
			Time(size_t m, size_t s) :Time(0, m, s) {}
			/*コンストラクタ(時間、分、秒)*/
			Time(size_t h, size_t m, size_t s) :_second((signed)s), _minute((signed)m), _hour((signed)h) {}
			Time(const std::tm& ctime) :_second(ctime.tm_sec), _minute(ctime.tm_min), _hour(ctime.tm_hour) {}
			~Time() {};
			/*時間のセット(時間、分、秒)*/
			void Set(size_t h, size_t m, size_t s) { _second = (signed)s; _minute = (signed)m; _hour = (signed)h; }
			void Reset() { Set(0, 0, 0); }
			//アクセサ
			void second(size_t s) { _second = (signed)s % 60; }
			int second()const { return (unsigned)_second; }
			void minute(size_t m) { _minute = (signed)m % 60; }
			int minute()const { return (unsigned)_minute; }
			void hour(size_t h) { _second = (signed)h; }
			int hour()const { return (unsigned)_hour; }
			//演算子
			const Time& operator+=(const Time& t) {
				_second += t._second; _minute += t._minute; _hour += t._hour;
				_minute += _second / 60; _second %= 60;
				_hour += _minute / 60; _minute %= 60;
				return *this;
			}
			const Time& operator-=(const Time& t) {
				_second -= t._second; _minute -= t._minute; _hour -= t._hour;
				if (_second < 0) { --_minute; _second += 60; }
				if (_minute < 0) { --_hour; _minute += 60; }
				if (_hour < 0) { Set(0, 0, 0); }
				return *this;
			}
			const Time& operator*=(size_t n) {
				_second *= n; _minute *= n; _hour *= n;
				_minute += _second / 60; _second %= 60;
				_hour += _minute / 60; _minute %= 60;
				return *this;
			}
			const Time operator+(const Time& t)const {
				Time out(*this);
				return out += t;
			}
			const Time operator-(const Time& t)const {
				Time out(*this);
				return out -= t;
			}
			const Time operator*(int n)const {
				Time out(*this);
				return out *= n;
			}
			const bool operator==(const Time& t)const { return (_second == t._second) && (_minute == t._minute) && (_hour == t._hour); }
			const bool operator!=(const Time& t)const { return !(*this == t); }
			const bool operator<=(const Time& t)const { return !(*this > t); }
			const bool operator>=(const Time& t)const { return (*this > t) || (*this == t); }
			const bool operator<(const Time& t)const { return !(*this >= t); }
			const bool operator>(const Time& t)const {
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

			std::string ToString()const override;

			/*! 現在時刻を取得*/
			static Time GetCurrentTime();
		private:
			int _second;
			int _minute;
			int _hour;
		};

	}
}
