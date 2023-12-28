#include <unordered_map>

#include "boost/multi_index_container.hpp"
#include "boost/multi_index/member.hpp"
#include "boost/multi_index/hashed_index.hpp"

#include "planeta/core/LogUtility.hpp"
#include "planeta/core/StringUtility.hpp"

#include "reflection.hpp"
#include "reflectable.hpp"
#include "reflection_exceptions.hpp"
#include "class_info_caller.hpp"
#include "reflectable_class_accessor.hpp"

namespace plnt::reflection {
	namespace {
		constexpr char *reflection_root_object_type_id(const_cast<char *>("reflectable"));
	}

	using namespace private_;
	using namespace util;
	namespace bmi = boost::multi_index;

	namespace tag {
		struct std_type_info { };

		struct object_type_id { };
	}

	//////////////////////////////////////////////////////////////////////////
	//Reflection::Impl_
	//////////////////////////////////////////////////////////////////////////
	class reflection::impl {
	public:
		using type_data_map_type = bmi::multi_index_container<std::unique_ptr<class_info>, bmi::indexed_by<
			                                                      bmi::hashed_unique<
				                                                      bmi::tag<tag::std_type_info>, bmi::member<
					                                                      class_info, type_info_wrapper, &
					                                                      class_info::this_t_info>,
				                                                      type_info_wrapper::hash>,
			                                                      bmi::hashed_unique<
				                                                      bmi::tag<tag::object_type_id>, bmi::member<
					                                                      class_info, std::string, &
					                                                      class_info::object_type_id>>
		                                                      >>;
		type_data_map_type type_data_map;
		bool is_initialized = false;
		class_info *reflection_root_class_info = nullptr;
		std::vector<std::string> error_que;
		//コンストラクタ
		impl() noexcept {
			//リフレクションルートを登録
			auto cf = std::make_unique<class_info>();
			cf->this_t_info = typeid(reflectable);
			cf->object_type_id = reflection_root_object_type_id;
			cf->super_t_info = typeid(void);
			reflection_root_class_info = cf.get();
			type_data_map.insert(std::move(cf));
		}

		//登録されたクラスデータから親子関係データを構築し、各種データを設定する
		void process_reflection_data() const {
			//親子のポインタをセット
			for (decltype(auto) std_ti_map = get_impl().type_data_map.get<tag::std_type_info>(); auto &&
			     this_class_info_u : std_ti_map) {
				//リフレクションルートでなかったら親クラスに関する処理を行う
				if (class_info *this_class_info = this_class_info_u.get(); this_class_info->object_type_id !=
					reflection_root_object_type_id) {
					//親クラスのデータセット
					const std::type_info &sti = this_class_info->super_t_info.get_type_info();
					auto it = std_ti_map.find(sti);
					if (it == std_ti_map.end()) {
						throw reflection_error(ConvertAndConnectToString(
							"ObjectTypeID\"", this_class_info->object_type_id, "\"の親クラス(std::type_info\"", sti.name(),
							"\")が登録されていません。"));
					}
					class_info *super_class_info = (*it).get();
					this_class_info->super_class_info = super_class_info;
					//親クラスに自分のクラスを登録
					super_class_info->child_t_info.emplace(this_class_info->this_t_info);
					super_class_info->child_class_info.emplace(this_class_info);
				}
			}
			//整合性確認と関数変数設定
			std::function<void(class_info &)> create_full_info = [&create_full_info](class_info &ci) {
				//親クラスが存在したら、自クラスに存在しない親クラスの変数を登録する
				if (ci.super_class_info != nullptr) {
					auto &super_class_info = *ci.super_class_info;
					//整合性確認
					if (const auto it = ci.super_class_info->child_t_info.find(ci.this_t_info); it == ci.
						super_class_info->child_t_info.end()) {
						//親クラスの子として自分が登録されていない
						throw reflection_error(ConvertAndConnectToString(
							"登録情報の整合性がありません。\"", ci.object_type_id, "\"の親クラス\"", super_class_info.object_type_id,
							"\"に自クラスが子クラスとして設定されていません。"));
					}
					//親クラスの変数関数追加
					for (auto &&var_prop : super_class_info.public_variable_property_info) {
						//親クラスにある変数のうち、自分のクラスにないものだったら追加
						if (auto var_id = var_prop.first; !ci.public_variable_property_info.contains(var_id)) {
							ci.public_variable_property_info.emplace(var_id, var_prop.second);
						}
					}
					//親クラスのコピーハンドラ継承
					ci.copy_handler_list.reserve(
						ci.copy_handler_list.size() + super_class_info.copy_handler_list.size());
					ci.copy_handler_list.insert(ci.copy_handler_list.begin(),
					                            super_class_info.copy_handler_list.begin(),
					                            super_class_info.copy_handler_list.end());
				}
				//子クラスに再帰的に適用
				for (auto &&cci : ci.child_class_info) {
					//整合性確認
					if (cci->super_t_info != ci.this_t_info) {
						//子クラスの親が自分でない
						throw reflection_error(ConvertAndConnectToString(
							"登録情報の整合性がありません。\"", ci.object_type_id, "\"の子クラス\"", cci->object_type_id,
							"\"に自クラスが親クラスとして設定されていません。\"", cci->super_class_info->object_type_id, "\"が設定されています。"));
					}
					create_full_info(const_cast<class_info &>(*cci));
				}
			};
			create_full_info(*reflection_root_class_info);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//Reflection
	//////////////////////////////////////////////////////////////////////////

	reflection::reflection() noexcept = default;

	reflection::impl &reflection::get_impl() {
		static impl impl;
		return impl;
	}


	std::shared_ptr<reflectable>
	reflection::create_object_by_object_type_id(const std::string &object_type_id) noexcept {
		decltype(auto) id_map = get_impl().type_data_map.get<tag::object_type_id>();
		if (const auto it = id_map.find(object_type_id); it == id_map.end()) {
			PE_LOG_ERROR("存在しないObject(ID:", object_type_id, ")が指定されました。");
			return nullptr;
		} else {
			if ((*it)->is_abstract) {
				PE_LOG_ERROR("ObjectTypeID\"", object_type_id, "\"に関連付けられたオブジェクトは抽象クラスのため、インスタンスを作成することはできません。");
				return nullptr;
			}
			return ((*it)->creator)();
		}
	}

	std::shared_ptr<reflectable> reflection::create_object_by_std_type_info(const std::type_info &t_info) noexcept {
		decltype(auto) std_ti_map = get_impl().type_data_map.get<tag::std_type_info>();
		if (const auto it = std_ti_map.find(t_info); it == std_ti_map.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。");
			return nullptr;
		} else {
			if ((*it)->is_abstract) {
				PE_LOG_ERROR("std::type_info\"", t_info.name(), "\"に関連付けられたオブジェクトは抽象クラスのため、インスタンスを作成することはできません。");
				return nullptr;
			}
			return ((*it)->creator)();
		}
	}

	std::string reflection::get_object_type_id_by_std_type_info(const std::type_info &t_info) {
		decltype(auto) std_ti_map = get_impl().type_data_map.get<tag::std_type_info>();
		const auto it = std_ti_map.find(t_info);
		if (it == std_ti_map.end()) {
			throw reflection_error(ConvertAndConnectToString("登録されていない型\"", t_info.name(), "\"が指定されました。"));
		}
		return (*it)->object_type_id;
	}

	const std::type_info &reflection::get_std_type_info_by_object_type_id(const std::string &id) {
		decltype(auto) id_map = get_impl().type_data_map.get<tag::object_type_id>();
		const auto it = id_map.find(id);
		if (it == id_map.end()) {
			throw reflection_error(ConvertAndConnectToString("登録されていない型ID\"", id, "\"が指定されました。"));
		}
		return (*it)->this_t_info.get_type_info();
	}

	std::shared_ptr<reflectable_class_accessor> reflection::get_reflectable_class_accessor(const std::type_info &ti) {
		auto *class_info = get_class_info_reflectable(ti);
		if (class_info == nullptr) {
			throw reflection_error(ConvertAndConnectToString("登録されていない型\"", ti.name(), "\"が指定されました。"));
		}
		return std::make_shared<reflectable_class_accessor>(class_info);
	}

	void reflection::bind_classes_to_lua(lua_State *l) {
		for (decltype(auto) id_map = get_impl().type_data_map.get<tag::object_type_id>(); auto &&ci : id_map) {
			ci->lua_binder(l);
		}
	}

	void reflection::register_object(const std::type_info &t_info, const std::string &id,
	                                 std::unique_ptr<class_info> &&class_info) {
		if (get_impl().is_initialized) {
			//初期化が終わっているときは例外を投げても問題ない
			throw reflection_error("オブジェクトの登録は初期化前に行われなければなりません。");
		}
		class_info->object_type_id = id;
		if (!get_impl().type_data_map.insert(std::move(class_info)).second) {
			//初期化前は例外が投げられないのでエラーqueに追加し、初期化時に確認する
			get_impl().error_que.push_back(
				ConvertAndConnectToString("型\"", t_info.name(), "\"が重複登録されました。(ID:\"", id, "\")"));
		}
	}

	void reflection::out_put_error_log(const std::string &detail, const std::string &place) {
		game::instance().log_manager()->Log(LogLevel::Error, place, detail);
	}

	const class_info *reflection::get_class_info_reflectable(const std::type_info &t_info) noexcept {
		decltype(auto) std_ti_map = get_impl().type_data_map.get<tag::std_type_info>();
		const auto it = std_ti_map.find(t_info);
		if (it == std_ti_map.end()) {
			PE_LOG_ERROR("存在しないObject(Type:", t_info.name(), ")が指定されました。登録されていないクラスの可能性があります。");
			return nullptr;
		}
		return (*it).get();
	}

	void reflection::initialize() {
		//エラーチェック
		if (!get_impl().error_que.empty()) {
			std::string e_str;
			for (auto &&err : get_impl().error_que) {
				e_str += "\t";
				e_str += err;
				e_str += "\n";
			}
			get_impl().error_que.clear();
			throw reflection_error(ConvertAndConnectToString("エラーが存在するため初期化を行えませんでした。エラー内容:\n", e_str));
		}
		//リフレクション情報の処理
		get_impl().process_reflection_data();
		get_impl().is_initialized = true;
	}

	size_t reflection::get_registered_class_count() noexcept {
		return get_impl().type_data_map.get<tag::std_type_info>().size();
	}
}
