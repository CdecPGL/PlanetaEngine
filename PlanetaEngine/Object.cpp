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
			size_t header_size = sizeof(size_t); //�w�b�_�T�C�Y�m�F
			size_t total_size = header_size + s; //�m�ۂ��鍇�v�T�C�Y
			void* ptr = malloc(total_size); //�f�[�^�T�C�Y�{�w�b�_�T�C�Y�̃��������m��
			assert(ptr != nullptr);
//			_reserved_memory_byte += total_size; //�m�ۍσT�C�Y�����Z
			(*reinterpret_cast<size_t*>(ptr)) = total_size; //�w�b�_�Ɋm�ۂ����T�C�Y���L�^
			return reinterpret_cast<char*>(ptr)+header_size; //�f�[�^�{�̂̃|�C���^��Ԃ�
			/*void* p = object_memory_allocator.Alloc(s);
			printf("allocated : %d , current_total : %d\n", s, (int)object_memory_allocator.GetAllocatedMemorySize());
			return p;*/
		}
		void Object::_deallocate(void* p){
			size_t header_size = sizeof(size_t); //�w�b�_�T�C�Y�m�F
			p = reinterpret_cast<char*>(p)-header_size; //�w�b�_�擪�ֈړ�
//			size_t s = (*reinterpret_cast<size_t*>(p)); //�T�C�Y���擾
//			_reserved_memory_byte -= s; //�m�ۍσT�C�Y�����Z
			return free(p);
			/*object_memory_allocator.Dealloc(p);
			printf("deallocated : %p , current_total : %d\n", p, (int)object_memory_allocator.GetAllocatedMemorySize());*/
		}
	}
}