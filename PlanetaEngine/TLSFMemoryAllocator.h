#pragma once
#include "IMemoryAllocator.h"
#include <cmath>
#include <vector>
#include<new>
#include "BadAllocException.h"

//http://marupeke296.com/ALG_No2_TLSFMemoryAllocator.html
//�|�C���^��32bit�ł��邱�Ƃ��O��ƂȂ��Ă���Bx64�v���O�����ɂ���ɂ͏C�����K�v

namespace planeta_engine{
	namespace core{
		namespace Allocators{
			class TLSFMemoryAllocator : public IMemoryAllcator{
			public:
				TLSFMemoryAllocator();
				~TLSFMemoryAllocator();
				bool ReserveMemory(size_t size)override; //�������̈�m�ہB�f�[�^�͑S���������
				void ReleaseMemory()override; //�m�ۂ����S���������J������
				size_t GetReservedMemorySize()const override{ return  _memory_pool_size; } //�m�ۂ��ꂽ�������̈�T�C�Y
				size_t GetUsingMemorySize()const override{ return _using_memory_size; } //�g�p����Ă��郁�����T�C�Y(�Ǘ��̈�܂�)
				size_t GetAllocatedMemoryCount()const override{ return _allocated_memory_count; } //���蓖�Ă�ꂽ�������̐�
				size_t GetAllocatedMemorySize()const override{ return _allcated_memory_size; } //���蓖�Ă�ꂽ�������T�C�Y���擾(�Ǘ��̈�͊܂܂Ȃ�)
				MemoryScanResult Scan()const override{ return DetailScan(); }
				struct MemoryDetailScanResult : public MemoryScanResult{
					bool inconsistency_at_dumy_count = false; //�_�~�[���̕s�����͂��邩
					bool inconsistency_at_free_block_list = false; //�t���[�u���b�N���X�g�ɓo�^����Ă���u���b�N���̕s�����͂��邩
					bool inconsistency_at_first_free_block_list_bit_array = false; //���t���[�u���b�N���X�g�r�b�g�̕s���������݂��邩
					bool inconsistency_at_second_free_block_list_bit_array = false; //���t���[�u���b�N���X�g�r�b�g�̕s���������݂��邩
					size_t total_block_count = 0; //���u���b�N��(�_�~�[����܂�)
					size_t free_block_count = 0; //�t���[�u���b�N��
					size_t active_block_count = 0; //�g�p���u���b�N��(�_�~�[��͊܂܂Ȃ�)
					size_t dumy_block_count = 0; //�_�~�[�u���b�N(�T�C�Y��0)��
				};
				MemoryDetailScanResult DetailScan()const; //�������̏ڍ׃X�L����

			private:
				class _Impl;
				_Impl* _impl;

				char* _memory_pool; //�m�ۂ����̈�̐擪�|�C���^
				size_t _memory_pool_size; //�m�ۂ����̈�̃T�C�Y

				size_t _using_memory_size; //�g�p���̃������T�C�Y(�Ǘ��̈���܂�)
				size_t _allcated_memory_size; //���蓖�Ă�ꂽ�������T�C�Y(�Ǘ��̈���܂܂Ȃ�)
				size_t _allocated_memory_count; //���蓖�Ă��������̑���

				void* _alloc(size_t)override; //���������蓖��
				void _dealloc(void* ptr)override; //���������蓖�ĉ���
			};
		}
	}
}