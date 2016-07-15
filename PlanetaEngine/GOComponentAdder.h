#pragma once

#include <vector>

#include "NonOwingPointer.h"
#include "GameObjectComponent.h"
#include "SystemLog.h"

namespace planeta {
	class GameObjectComponentHolder;
	class GOComponentAdder {
	public:
		GOComponentAdder(GameObjectComponentHolder& com_holder,std::vector<std::pair<std::string,std::shared_ptr<GameObjectComponent>>>& alias_goc_list_buf);
		/*GOコンポーネントをIDで追加し名前を付ける*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id, const std::string& alias);
		/*GOコンポーネントをIDで追加*/
		util::NonOwingPointer<GameObjectComponent> CreateAndAddComponent(const std::string& com_id);
		/*GOコンポーネントを型で追加し名前を付ける*/
		template<class ComT>
		util::NonOwingPointer<ComT> CreateAndAddComponent(const std::string& alias) {
			static_assert(std::is_base_of<GameObjectComponent, ComT>::value, "ComT must derive GAmeObjectComponent.");
			auto com = std::make_shared<ComT>();
			alias_goc_list_buf_.push_back(std::make_pair(alias, com));
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
		std::vector<std::pair<std::string, std::shared_ptr<GameObjectComponent>>>& alias_goc_list_buf_;
		void AddComponentToHolder_(const std::shared_ptr<GameObjectComponent>& com,const std::type_info& tinfo);
	};
}
