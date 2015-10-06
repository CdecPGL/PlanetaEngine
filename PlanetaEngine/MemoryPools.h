#pragma once
#include "IMemoryAllocator.h"

namespace planeta_engine{
	namespace core{
		//�������[�v�[���B�Ǘ��̓������[�}�l�[�W�����s��
		namespace MemoryPools{
			extern IMemoryAllcator* engine_memory_pool;
			extern IMemoryAllcator* game_object_memory_pool;
			extern IMemoryAllcator* process_memory_pool;
			extern IMemoryAllcator* ui_memory_pool;
		}
	}
}