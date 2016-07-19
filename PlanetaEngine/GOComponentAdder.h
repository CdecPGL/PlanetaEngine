#pragma once

#include <vector>

#include "NonOwingPointer.h"
#include "GameObjectComponent.h"
#include "SystemLog.h"

namespace planeta {
	class GameObjectComponentHolder;
	/*! @brief ゲームオブジェクトコンポーネントを追加するためのクラス

	GameObject::AddConponentsProcの引数として渡される。
	*/
	class GOComponentAdder {
	public:
		GOComponentAdder(GameObjectComponentHolder& com_holder);
		/*! @brief ゲームオブジェクトコンポーネントをIDで追加し名前を付ける

			該当する型がシステムに登録されている必要がある。
			名前は定義ファイルからのデータのロード時、コンポーネントにデータを振り分けるために使用される。
		*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id, const std::string& alias);
		/*! @brief ゲームオブジェクトコンポーネントをIDで追加する

		該当する型がシステムに登録されている必要がある。
		*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id);
		/*! @brief ゲームオブジェクトコンポーネントを型で追加し名前を付ける

		名前は定義ファイルからのデータのロード時、コンポーネントにデータを振り分けるために使用される。
		*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent(const std::string& alias) {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");
			auto com = std::make_shared<ComT>();
			if (!AddComponentToHolder_(com, typeid(ComT), alias)) {
				return nullptr;
			}
			return com;
		}
		/*! @brief ゲームオブジェクトコンポーネントを型で追加する

		システムに該当する型が登録されている必要がある。
		名前は定義ファイルからのデータのロード時、コンポーネントにデータを振り分けるために使用される。
		*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto ret = GetDefaultComID_(typeid(ComT));
			if (ret.first) {
				return CreateAndAddComponent<ComT>(ret.second);
			} else {
				PE_LOG_ERROR("デフォルトのコンポーネントエイリアスを取得できませんでした。");
				return nullptr;
			}
		}

	private:
		GameObjectComponentHolder& com_holder_;
		std::pair<bool,std::string> GetDefaultComID_(const std::type_info& tinfo);
		bool AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com,const std::type_info& tinfo, const std::string& alias);
	};
}
