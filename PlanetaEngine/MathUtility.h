#pragma once

namespace planeta {
	namespace math {
		/*�l��2�ׂ̂��搔�����ׂ�*/
		bool is2Pow(unsigned int num);
		/*�l�ȏ�ōŏ���2�ׂ̂��搔�𓾂�*/
		unsigned int getMin2Pow(unsigned int num);
		/*double��0�ł��邩��Ό덷���w�肵�Ĕ���*/
		bool DoubleIsZero(double d, double epsilon = 1e-5);
		/*double������������Ό덷���w�肵�Ĕ���*/
		bool DoubleIsEqual(double d1, double d2, double epsilon = 1e-5);
	}
}