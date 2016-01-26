#include "LuaBindClassFunctions.h"
#include "LuaIntf.h"
#include "SceneAccessorForGameProcess.h"
#include <vector>
#include <unordered_map>

namespace LuaIntf {
	LUA_USING_SHARED_PTR_TYPE(planeta_engine::utility::WeakPointer)
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::unordered_map)
}

namespace planeta_engine {
	namespace lua_bind_class_functions {

		void BindSceneAccessorForGameProcess(lua_State* L) {
			
			LuaIntf::LuaBinding(L).beginClass<SceneAccessorForGameProcess>("SceneAccessorForGameProcess")
				.addFunction("CreateGameObject",&SceneAccessorForGameProcess::CreateGameObject)
				.addFunction("CreateGameObjectWithName", &SceneAccessorForGameProcess::CreateGameObjectWithName)
				.addFunction("CreateAndActivateGameObject", &SceneAccessorForGameProcess::CreateAndActivateGameObject)
				.addFunction("CreateAndActivateGameObjectWithName", &SceneAccessorForGameProcess::CreateAndActivateGameObjectWithName)
				.endClass();
		}

	}
}