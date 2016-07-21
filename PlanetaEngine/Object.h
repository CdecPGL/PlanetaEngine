#pragma once
#include <string>
#include <typeinfo>
#include <memory>
#include "Reflection.h"

#undef max //windows.h�Œ�`����Ă���max�}�N���𖳌���(std::numeric_limits<size_t>::max()�̂���)

namespace planeta {
	namespace core {
		/*! @brief �S�ẴN���X�̊��ƂȂ��{�N���X
		
			�^���Ǘ��A���t���N�V�����V�X�e���Ȃǂ�񋟂���B�����̋@�\���K�v�Ȃ��ꍇ�͌p������K�v�͂Ȃ�
		*/
		class Object {
		public:
			//! �f�t�H���g�R���X�g���N�^
			Object() = default;
			//! �f�X�g���N�^
			virtual ~Object() = default;
			//! �C���X�^���X���������m�F
			bool Equals(const Object* o)const { return o == this; }
			//! �C���X�^���X���������m�F
			bool Equals(const Object& o)const { return &o == this; }
			//! �����񉻂���B�p�����邱�ƂœƎ��̕����񂩋@�\��ǉ��\�B
			virtual std::string ToString()const { return typeid(*this).name(); }
		};
	}
}

