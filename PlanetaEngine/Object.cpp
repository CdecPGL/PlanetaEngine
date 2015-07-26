#include "Object.h"

namespace PlanetaEngine{
	namespace Core{
		size_t Object::_reserved_memory_byte = 0;
		void* Object::_allocate(size_t s){
			size_t header_size = sizeof(size_t); //�w�b�_�T�C�Y�m�F
			size_t total_size = header_size + s; //�m�ۂ��鍇�v�T�C�Y
			void* ptr = malloc(total_size); //�f�[�^�T�C�Y�{�w�b�_�T�C�Y�̃��������m��
			_reserved_memory_byte += total_size; //�m�ۍσT�C�Y�����Z
			(*reinterpret_cast<size_t*>(ptr)) = total_size; //�w�b�_�Ɋm�ۂ����T�C�Y���L�^
			return reinterpret_cast<char*>(ptr)+header_size; //�f�[�^�{�̂̃|�C���^��Ԃ�
		}
		void Object::_deallocate(void* p){
			size_t header_size = sizeof(size_t); //�w�b�_�T�C�Y�m�F
			p = reinterpret_cast<char*>(p)-header_size; //�w�b�_�擪�ֈړ�
			size_t s = (*reinterpret_cast<size_t*>(p)); //�T�C�Y���擾
			_reserved_memory_byte -= s; //�m�ۍσT�C�Y�����Z
			return free(p);
		}
	}
}