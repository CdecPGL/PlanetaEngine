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

//GameObject‚ðƒVƒXƒeƒ€‚É“o˜^‚·‚é(ID,Œ^)
#define PE_REGISTER_OBJECT(id, type)\
namespace {planeta_engine::core::GlobalObjectRegisterer<id,type> globalobject_registerer();}