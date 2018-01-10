#pragma once

#include "planeta/PETime.hpp"

namespace plnt {
	/*! �p�t�H�[�}���X�}�l�[�W���̌��J�C���^�[�t�F�C�X�B�p�t�H�[�}���X�̌v���⒲�����s���B*/
	class IPerformanceManager {
	public:
		virtual ~IPerformanceManager() = 0 {};
		/*! �N������̌o�߃t���[�����擾����*/
		virtual size_t GetCurrentFrameCount()const = 0;
		/*! �N������̌o�ߎ��Ԃ��擾����*/
		virtual const util::Time GetCurrentTimeCount()const = 0;
	};
}