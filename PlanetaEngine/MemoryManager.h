#pragma once
#include <stdexcept>

#include "IMemoryAllocator.h"

//����ς肠�Ƃɂ��悤

namespace planeta_engine{
	namespace core{
		class MemoryManager{
		public:
			static void CreateInstance(){ if (_instance == nullptr){ _instance = new MemoryManager; } }
			static MemoryManager& GetInstance(){ if (_instance == nullptr){ throw std::logic_error("Instance of MemoryManager is not created."); }return *_instance; }
			static void DestroyInstance(){ if (_instance){ delete _instance; }_instance = nullptr; }
			bool Init(size_t eg_pool_size, size_t go_pool_size, size_t pr_pool_size, size_t ui_pool_size); //������(�G���W���̈�A�Q�[���I�u�W�F�N�g�̈�A�v���Z�X�̈�AUI�̈�)
			bool End();
			bool Process();
		private:
			static MemoryManager* _instance;
			MemoryManager();
			~MemoryManager();
		};
	}
}