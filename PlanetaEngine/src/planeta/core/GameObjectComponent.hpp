﻿#pragma once

#include <memory>
#include <functional>
#include <unordered_set>

#include "planeta/reflection/ReflectionAccessible.hpp"

#include "Object.hpp"
#include "NonCopyable.hpp"
#include "WeakPointer.hpp"
#include "NonOwingPointer.hpp"
#include "GOComponentGetter.hpp"
#include "IScene.hpp"

namespace plnt {
	class SceneAccessorForGameObject;
	namespace private_{
		class ISceneInternal;
		struct GameObjectComponentSetUpData;
	}
	class IGameObject;
	/*! @brief ゲームオブジェクトコンポーネントの基底クラス
		@attention クラス宣言の先頭にPE_CLONABLE_GAMEOBJECTCOMPONENTマクロ、抽象クラスならPE_COPIABLE_GAMEOBJECTCOMPONENTを記述しなければならない。
		@warning ゲームオブジェクトコンポーネントの定義の際には、このクラスを直接継承してはいけない。代わりにGameObjectStandardComponentを継承すること。
	*/
	class GameObjectComponent : public Object, public reflection::ReflectionAccessible, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(GameObjectComponent);
	public:
		using Super = Object;
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SetSceneAndHolderGOData(const private_::GameObjectComponentSetUpData& resistration_data);

		/*! @brief ゲームオブジェクトコンポーネントが使用可能か
			
			所有されているゲームオブジェクトが破棄されているときや、正しく初期化されていないときにfalseを返す。
		*/
		bool is_valied()const { return is_valied_; }
		//! 所有されているゲームオブジェクトがシーン内で有効状態か
		bool is_active()const { return is_active_; }
		/*! @brief 所有されているゲームオブジェクトへの参照*/
		IGameObject& game_object();
		/*! ラベルの取得*/
		const std::unordered_set<std::string>& labels()const;
		/*! ラベルの設定*/
		void labels(const std::unordered_set<std::string>& l);

		/*システム関数(GameObjectBaseから呼び出される)*/
		/*! @brief 保持されているゲームオブジェクトのほかのコンポーネントを取得する。

		この関数内でコンポーネントを初期化することもできるが、特定のゲームオブジェクトインスタンスに依存する処理(thisのセットなど)は行ってはいけない。
		先頭で親クラスの同関数を呼び出す必要がある。
		*/

		virtual bool GetOtherComponentsProc(const GOComponentGetter& com_getter) { com_getter; return true; }
		//初期化
		void Initialize();
		void Activate();
		void InActivate();
		void Finalize();
	protected:
		//オーバーライド可能関数
		/*イベント関数*/
		/*! @brief 初期化時イベント関数

			所属するゲームオブジェクトが初期化されたときに呼び出される。ほかのコンポーネントの取得もここで行う。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void OnInitialized() {};
		/*! @brief 有効化時イベント関数

			所属するゲームオブジェクトが有効化されたときに呼び出される。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void OnActivated() {};
		/*! @brief 無効化時イベント関数

		所属するゲームオブジェクトが無効化されたときに呼び出される。<br/>
		末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void OnInactivated() {} //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		/*! @brief 破棄時イベント関数

			所属するゲームオブジェクトが破棄される直前に呼び出される。<br/>
			末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void OnFinalized()noexcept {} //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
		/*! シーンへのアクセス*/
		IScene& scene();
		/*! 自身のshared_ptrにアクセス(現在は、型変換のチェックはされない)*/
		template<class T>
		std::shared_ptr<T> shared_this() {
			return std::static_pointer_cast<T>(shared_from_this());
		}
		/*! 自身のconstなshared_ptrにアクセス(現在は、型変換のチェックはされない)*/
		template<class T>
		std::shared_ptr<const T> shared_this()const {
			return std::static_pointer_cast<const T>(shared_from_this());
		}
	private:
		bool is_valied_ = false;
		bool is_active_ = false;
		std::unordered_set<std::string> labels_;

		NonOwingPointer<IGameObject> game_object_;
		WeakPointer<IScene> scene_;

		/*特別設定関数*/
		virtual void SetSceneInternalInterface(const WeakPointer<private_::ISceneInternal>& scene_data) = 0;
	};
	PE_REFLECTABLE_CLASS(GameObjectComponent);
}

#define PE_GAMEOBJECTCOMPONENT_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
