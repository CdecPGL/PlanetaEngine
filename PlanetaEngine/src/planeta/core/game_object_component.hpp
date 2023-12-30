#pragma once

#include <memory>
#include <functional>
#include <unordered_set>

#include "../reflection/reflection_accessible.hpp"

#include "Object.hpp"
#include "non_copyable.hpp"
#include "WeakPointer.hpp"
#include "non_owing_pointer.hpp"
#include "go_component_getter.hpp"
#include "i_scene.hpp"

namespace plnt {
	class SceneAccessorForGameObject;

	namespace private_ {
		class i_scene_internal;
		struct game_object_component_set_up_data;
	}

	class i_game_object;
	/*! @brief ゲームオブジェクトコンポーネントの基底クラス
		@attention クラス宣言の先頭にPE_CLONABLE_GAMEOBJECTCOMPONENTマクロ、抽象クラスならPE_COPIABLE_GAMEOBJECTCOMPONENTを記述しなければならない。
		@warning ゲームオブジェクトコンポーネントの定義の際には、このクラスを直接継承してはいけない。代わりにGameObjectStandardComponentを継承すること。
	*/
	class game_object_component : public object, public reflection::reflection_accessible,
	                            public std::enable_shared_from_this<game_object_component>,
	                            util::non_copyable<game_object_component> {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(game_object_component);

	public:
		using super = object;
		game_object_component() = default;
		game_object_component(const game_object_component&) = delete;
		game_object_component(game_object_component&&) = delete;
		~game_object_component() override = default;
		game_object_component& operator=(const game_object_component&) = delete;
		game_object_component& operator=(game_object_component&&) = delete;

		bool set_scene_and_holder_go_data(const private_::game_object_component_set_up_data &resister_data);

		/*! @brief ゲームオブジェクトコンポーネントが使用可能か
			
			所有されているゲームオブジェクトが破棄されているときや、正しく初期化されていないときにfalseを返す。
		*/
		bool is_valid() const { return is_valid_; }
		//! 所有されているゲームオブジェクトがシーン内で有効状態か
		bool is_active() const { return is_active_; }
		/*! @brief 所有されているゲームオブジェクトへの参照*/
		i_game_object &game_object() const;
		/*! ラベルの取得*/
		const std::unordered_set<std::string> &labels() const;
		/*! ラベルの設定*/
		void labels(const std::unordered_set<std::string> &l);

		/*システム関数(GameObjectBaseから呼び出される)*/
		/*! @brief 保持されているゲームオブジェクトのほかのコンポーネントを取得する。

		この関数内でコンポーネントを初期化することもできるが、特定のゲームオブジェクトインスタンスに依存する処理(thisのセットなど)は行ってはいけない。
		先頭で親クラスの同関数を呼び出す必要がある。
		*/

		virtual bool get_other_components_proc([[maybe_unused]] const go_component_getter &com_getter) { return true; }

		//初期化
		void initialize();
		void activate();
		void in_activate();
		void finalize();

	protected:
		//オーバーライド可能関数
		/*イベント関数*/
		/*! @brief 初期化時イベント関数

			所属するゲームオブジェクトが初期化されたときに呼び出される。ほかのコンポーネントの取得もここで行う。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void on_initialized() { }
		/*! @brief 有効化時イベント関数

			所属するゲームオブジェクトが有効化されたときに呼び出される。<br/>
			先頭で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void on_activated() { }
		/*! @brief 無効化時イベント関数

		所属するゲームオブジェクトが無効化されたときに呼び出される。<br/>
		末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void on_inactivated() { } //所属するゲームオブジェクトがシーンから登録解除されるときに呼び出され(システム関数)
		/*! @brief 破棄時イベント関数

			所属するゲームオブジェクトが破棄される直前に呼び出される。<br/>
			末尾で親クラスの同関数を呼び出す必要がある。
		*/
		virtual void on_finalized() noexcept { } //所属するゲームオブジェクトが破棄されるときに呼び出される(システム関数)
		/*! シーンへのアクセス*/
		i_scene &scene() const;
		/*! 自身のshared_ptrにアクセス(現在は、型変換のチェックはされない)*/
		template <class T>
		std::shared_ptr<T> shared_this() { return std::static_pointer_cast<T>(shared_from_this()); }

		/*! 自身のconstなshared_ptrにアクセス(現在は、型変換のチェックはされない)*/
		template <class T>
		std::shared_ptr<const T> shared_this() const { return std::static_pointer_cast<const T>(shared_from_this()); }

	private:
		bool is_valid_ = false;
		bool is_active_ = false;
		std::unordered_set<std::string> labels_;

		non_owing_pointer<i_game_object> game_object_;
		WeakPointer<i_scene> scene_;

		/*特別設定関数*/
		virtual void set_scene_internal_interface(const WeakPointer<private_::i_scene_internal> &scene_data) = 0;
	};

	PE_REFLECTABLE_CLASS(game_object_component);
}

#define PE_GAMEOBJECTCOMPONENT_CLASS(type)\
PE_REFLECTABLE_CLASS(type)
