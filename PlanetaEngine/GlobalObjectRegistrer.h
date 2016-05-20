#pragma once

#include "GlobalObjectFactory.h"

namespace planeta_engine {
	template<const char* ObjectID, class ObjectType>
	class GlobalObjectRegisterer {
	public:
		GlobalObjectRegisterer() {
			GlobalObjectFactory::GetInstance().RegisterObject<ObjectType>(ObjectID);
		}
	};
}

//GameObject���V�X�e���ɓo�^����(ID,�^)
#define PE_REGISTER_OBJECT(id, type)\
namespace {planeta_engine::core::GlobalObjectRegisterer<id,type> globalobject_registerer();}