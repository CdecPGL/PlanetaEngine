#include "planeta/buildin/TInstant.hpp"

namespace plnt {
	void TInstant::Update() {
		function_();
		if (number_of_times_implemented_ > 0) {
			++count_of_times_implemented_;
			if (number_of_times_implemented_ >= count_of_times_implemented_) {
				//実行回数に達したのでプロセスを削除する
				Dispose();
			}
		}
	}
}