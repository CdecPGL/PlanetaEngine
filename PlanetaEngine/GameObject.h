#pragma once

#include <type_traits>
#include "GameObjectBase.h"
#include "GameObjectInterface.h"
#include "MetaprogrammingUtility.h"

namespace planeta_engine {
	//ゲームオブジェクトを定義するときは、このクラスを継承する。テンプレート引数にはGameObjectInterfaceを任意個指定する。
	template<typename... GOI>
	class GameObject : public GameObjectBase , public GOI... {
		//GOI...がGameObjectInterfaceの派生クラスか確認。
		template<typename Arg> struct GOI_is_base_of :public std::is_base_of<GameObjectInterface, Arg> {};
		static_assert(mp_utiliey::AllOf<GOI_is_base_of, GOI...>::value == false, "GOI must derive GameObjectInterface.");
	public:
		GameObject():GOI(*this)... {}
	private:
		//システム関数の隠蔽
		bool ProcessActivation();
		bool ProcessInactivation();
		bool ProcessDisposal();
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		void SetSceneData(const utility::WeakPointer<core::SceneData>& scene_data);
	};
}

//GameObjectをシステムに登録する(型)
#define PE_REGISTER_GAMEOBJECT(type)\
PE_REGISTER_OBJECT(type,planeta_engine::GameObjectBase)
