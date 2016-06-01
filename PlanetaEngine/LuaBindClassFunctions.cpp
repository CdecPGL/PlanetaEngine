#include "LuaBindClassFunctions.h"
#include "LuaIntf.h"
#include <vector>
#include <unordered_map>

namespace LuaIntf {
	/*LUA_USING_SHARED_PTR_TYPE(planeta_engine::utility::WeakPointer)
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::unordered_map)*/
}

namespace planeta {
	namespace lua_bind_class_functions {

		void BindSceneAccessorForTask(lua_State* L) {
			/*
			LuaIntf::LuaBinding(L).beginClass<SceneAccessorForTask>("SceneAccessorForTask")
				.addFunction("CreateGameObject",&SceneAccessorForTask::CreateGameObject)
				.addFunction("CreateGameObjectWithName", &SceneAccessorForTask::CreateGameObjectWithName)
				.addFunction("CreateAndActivateGameObject", &SceneAccessorForTask::CreateAndActivateGameObject)
				.addFunction("CreateAndActivateGameObjectWithName", &SceneAccessorForTask::CreateAndActivateGameObjectWithName)
				.endClass();*/
		}

	}
}