#pragma once
#include <memory>
#include "ResourceBase.h"

namespace planeta_engine {
	namespace core {
		/*���̊֐����g���ă��\�[�X���쐬���邱�ƂŃ��\�[�X�̔j���������ōs����悤�ɂȂ�*/
		template<class Res>
		std::shared_ptr<Res> MakeResource() { return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; })); }
	}
}