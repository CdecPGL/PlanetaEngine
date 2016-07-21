﻿#pragma once

#include <type_traits>
#include "GameObjectBase.h"
#include "GameObjectInterface.h"
#include "MetaprogrammingUtility.h"
#include "GOComponentAdder.h"

namespace planeta {
	/*! @brief ゲームオブジェクトの基底となるクラス。ゲームオブジェクトを定義するときはこのクラスを継承する。
	
		テンプレート引数にはGameObjectInterfaceを任意個指定する。
	*/
	template<typename... GOI>
	class GameObject : public GameObjectBase , public GOI... {
		//GOI...がGameObjectInterfaceの派生クラスか確認。
		template<typename Arg> struct GOI_is_base_of :public std::is_base_of<GameObjectInterface<Arg>, Arg> {};
		static_assert(mp_utiliey::AllOf<GOI_is_base_of, GOI...>::value == true, "GOI must derive GameObjectInterface.");
	public:
		//! 規定のコンストラクタ
		GameObject():GOI(*this)... {}
	private:
		//システム関数の隠蔽
		bool ProcessInstantiation();
		bool ProcessLoading();
		bool ProcessActivation();
		bool ProcessInactivation();
		bool ProcessDisposal();
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data);
	};
}

/*! @def
	GameObjectをシステムに登録する
	@param(type) 登録したい型
*/
#define PE_REGISTER_GAMEOBJECT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectBase)
