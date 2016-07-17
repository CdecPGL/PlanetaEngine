#pragma once

#include <vector>

#include "NonOwingPointer.h"
#include "GameObjectComponent.h"
#include "SystemLog.h"

namespace planeta {
	class GameObjectComponentHolder;
	class GOComponentAdder {
	public:
		GOComponentAdder(GameObjectComponentHolder& com_holder);
		/*GOコンポーネントをIDで追加し名前を付ける*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id, const std::string& alias);
		/*GOコンポーネントをIDで追加*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id);
		/*GOコンポーネントを型で追加し名前を付ける*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent(const std::string& alias) {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");
			auto com = std::make_shared<ComT>();
			if (!AddComponentToHolder_(com, typeid(ComT), alias)) {
				return nullptr;
			}
			return com;
		}
		/*GOコンポーネントを型で追加*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent() {
			auto ret = GetDefaultComID_(typeid(ComT));
			if (ret.first) {
				return CreateAndAddComponent(ret.second);
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
