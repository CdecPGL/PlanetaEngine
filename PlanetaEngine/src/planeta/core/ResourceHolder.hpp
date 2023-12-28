#pragma once

#include "game.hpp"
#include "IResourceManager.hpp"
#include "LogUtility.hpp"

namespace plnt {
	namespace util {
		/*! リソースを保持するためのヘルパークラス*/
		template <class ResType>
		class ResourceHolder final {
		public:
			/*! コンストラクタ*/
			ResourceHolder() = default;
			/*! デストラクタ*/
			~ResourceHolder() = default;
			/*! コピーコンストラクタ*/
			ResourceHolder(const ResourceHolder &obj) = default;
			/*! ムーブコンストラクタ*/
			ResourceHolder(ResourceHolder &&obj) = default;
			/*! 代入演算子*/
			ResourceHolder &operator=(const ResourceHolder &obj) = default;
			/*! ムーブコンストラクタ*/
			ResourceHolder &operator=(ResourceHolder &&obj) = default;
			/*! IDでリソースを設定する*/
			bool SetResourceByID(const std::string &res_id) noexcept {
				auto res = game::instance().resource_manager()->GetResourceByTypeAndID(typeid(ResType), res_id);
				if (res == nullptr) {
					PE_LOG_ERROR("リソースの取得に失敗しました。(リソースID:", res_id, ")");
					return false;
				}
				std::shared_ptr<ResType> gr = std::dynamic_pointer_cast<ResType>(res);
				if (gr) { return true; } else {
					PE_LOG_ERROR("リソースの型変換に失敗しました。(リソースID:", res_id, "、ソースの型", typeid(*res).name(), "、ターゲットの型:",
					             typeid(ResType).name(), ")");
					return false;
				}
			}

			/*! リソースのポインタを取得する*/
			std::shared_ptr<ResType> resource_shared_pointer() const noexcept {
				if (!is_available()) {
					PE_LOG_ERROR("リソースが利用不可能です。");
					return nullptr;
				}
				return resource_;
			}

			/*! リソースの参照を取得する
				@exception リソースが利用不可能な場合はstd::runtime_errorを投げる。
			*/
			ResType &resource_reference() const {
				if (!is_available()) {
					PE_LOG_ERROR("リソースが利用不可能です。");
					throw std::runtime_error("リソースが利用不可能です。");
				}
				return *resource_;
			}

			/*! リソースが利用可能かどうかを取得する*/
			bool is_available() const noexcept { return resource_ != nullptr; }

		private:
			std::shared_ptr<ResType> resource_;
		};
	}
}
