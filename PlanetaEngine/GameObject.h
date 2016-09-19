#pragma once

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
		using Super = GameObjectBase;
		//! 規定のコンストラクタ
		GameObject():GOI(*this)... {}
	private:
		//システム関数の隠蔽
		bool ProcessInstantiation();
		bool ProcessClonation(const std::shared_ptr<GameObjectBase>&);
		bool ProcessLoading();
		bool ProcessActivation();
		bool ProcessInactivation();
		bool ProcessDisposal();
		void SetManagerConnection(std::unique_ptr<GameObjectManagerConnection>&& mgr_cnctn);
		void SetSceneData(const WeakPointer<private_::SceneData>& scene_data);
		void SetSceneAndGODataToCOmponents();
	};
}

#define PE_GAMEOBJECT_CLASS(type)\
PE_REFLECTABLE_CLASS_SPECIFY_SUPER(type,planeta::GameObjectBase)
