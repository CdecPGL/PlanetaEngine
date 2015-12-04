#include "Object.h"
#include "TLSFMemoryAllocator.h"
#include <cassert>

namespace planeta_engine{
	namespace core{
		namespace {
//			TLSFMemoryAllocator object_memory_allocator(1024*1024);
		}

//		const IMemoryAllcator& Object::memory_allocator() { return object_memory_allocator; }

		void* Object::_allocate(size_t s){
			size_t header_size = sizeof(size_t); //ヘッダサイズ確認
			size_t total_size = header_size + s; //確保する合計サイズ
			void* ptr = malloc(total_size); //データサイズ＋ヘッダサイズのメモリを確保
			assert(ptr != nullptr);
//			_reserved_memory_byte += total_size; //確保済サイズを加算
			(*reinterpret_cast<size_t*>(ptr)) = total_size; //ヘッダに確保したサイズを記録
			return reinterpret_cast<char*>(ptr)+header_size; //データ本体のポインタを返す
			/*void* p = object_memory_allocator.Alloc(s);
			printf("allocated : %d , current_total : %d\n", s, (int)object_memory_allocator.GetAllocatedMemorySize());
			return p;*/
		}
		void Object::_deallocate(void* p){
			size_t header_size = sizeof(size_t); //ヘッダサイズ確認
			p = reinterpret_cast<char*>(p)-header_size; //ヘッダ先頭へ移動
//			size_t s = (*reinterpret_cast<size_t*>(p)); //サイズ情報取得
//			_reserved_memory_byte -= s; //確保済サイズを減算
			return free(p);
			/*object_memory_allocator.Dealloc(p);
			printf("deallocated : %p , current_total : %d\n", p, (int)object_memory_allocator.GetAllocatedMemorySize());*/
		}
	}
}