﻿#pragma once

#include "game.hpp"
#include "i_resource_manager.hpp"
#include "log_utility.hpp"

namespace plnt::util {
	/*! リソースを保持するためのヘルパークラス*/
	template <class ResType>
	class resource_holder final {
	public:
		/*! コンストラクタ*/
		resource_holder() = default;
		/*! デストラクタ*/
		~resource_holder() = default;
		/*! コピーコンストラクタ*/
		resource_holder(const resource_holder &obj) = default;
		/*! ムーブコンストラクタ*/
		resource_holder(resource_holder &&obj) = default;
		/*! 代入演算子*/
		resource_holder &operator=(const resource_holder &obj) = default;
		/*! ムーブコンストラクタ*/
		resource_holder &operator=(resource_holder &&obj) = default;
		/*! IDでリソースを設定する*/
		[[nodiscard]] bool set_resource_by_id(const std::string &res_id) const noexcept {
			const auto res = game::instance().resource_manager()->get_resource_by_type_and_id(typeid(ResType), res_id);
			if (res == nullptr) {
				PE_LOG_ERROR("リソースの取得に失敗しました。(リソースID:", res_id, ")");
				return false;
			}
			if (std::shared_ptr<ResType> gr = std::dynamic_pointer_cast<ResType>(res)) { return true; }
			// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
			PE_LOG_ERROR("リソースの型変換に失敗しました。(リソースID:", res_id, "、ソースの型", typeid(*res).name(), "、ターゲットの型:",
			             typeid(ResType).name(), ")");
			return false;
		}

		/*! リソースのポインタを取得する*/
		[[nodiscard]] std::shared_ptr<ResType> resource_shared_pointer() const noexcept {
			if (!is_available()) {
				PE_LOG_ERROR("リソースが利用不可能です。");
				return nullptr;
			}
			return resource_;
		}

		/*! リソースの参照を取得する
				@exception リソースが利用不可能な場合はstd::runtime_errorを投げる。
			*/
		[[nodiscard]] ResType &resource_reference() const {
			if (!is_available()) {
				PE_LOG_ERROR("リソースが利用不可能です。");
				throw std::runtime_error("リソースが利用不可能です。");
			}
			return *resource_;
		}

		/*! リソースが利用可能かどうかを取得する*/
		[[nodiscard]] bool is_available() const noexcept { return resource_ != nullptr; }

	private:
		std::shared_ptr<ResType> resource_;
	};
}
