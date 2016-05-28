#pragma once

#include "GlobalObjectFactory.h"

namespace planeta_engine {
	namespace core {
		template<typename ObjectType>
		class GlobalObjectRegisterer {
		public:
			GlobalObjectRegisterer(const char* object_id, const std::function<std::shared_ptr<Object>()>& creator) {
				GlobalObjectFactory::instance().RegisterObject<ObjectType>(object_id, creator);
			}
		};
	}
}

//Object‚ğƒVƒXƒeƒ€‚É“o˜^‚·‚é(Œ^,Œp³‚·‚×‚«Œ^)
#define PE_REGISTER_OBJECT(type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta_engine::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (#type,[]{return std::make_shared<type>();});}

//ID‚ğw’è‚µ‚ÄObject‚ğƒVƒXƒeƒ€‚É“o˜^‚·‚é(ID<•¶š—ñƒŠƒeƒ‰ƒ‹>,Œ^,Œp³‚·‚×‚«Œ^)
#define PE_REGISTER_OBJECT_WITH_ID(id,type,base_check)\
static_assert(std::is_base_of<base_check,type>::value == true, #type " must derive " #base_check);\
namespace {planeta_engine::core::GlobalObjectRegisterer<type> globalobject_registerer_##type (id,[]{return std::make_shared<type>();});}