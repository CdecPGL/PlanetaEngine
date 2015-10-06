#include "MemoryPools.h"

namespace planeta_engine{
	namespace core{
		namespace MemoryPools{
			IMemoryAllcator* engine_memory_pool = nullptr;
			IMemoryAllcator* game_object_memory_pool = nullptr;
			IMemoryAllcator* process_memory_pool = nullptr;
			IMemoryAllcator* ui_memory_pool = nullptr;
		}
	}
}