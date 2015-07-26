#include "GameObjectAllocator.h"

#include "MemoryPools.h"

namespace PlanetaEngine{
	namespace Core{
		namespace Allocators{
			void* allocate_game_object(size_t size){
				return MemoryPools::game_object_memory_pool->Alloc(size);
			}
			void deallocate_game_object(void* ptr){
				MemoryPools::game_object_memory_pool->Dealloc(ptr);
			}
		}
	}
}