#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <typeindex>
#include "boost/lexical_cast.hpp"
#include "MetaprogrammingUtility.h"
#include "SystemLog.h"

/*未完成*/
namespace planeta {
	namespace util {
		class Reflection {
		public:
			Reflection() = default;
			virtual ~Reflection() = default;
			template<typename ... TArgs>
			bool Call(const std::string& func_name, TArgs ... params) {
				//子クラスから順に処理していく。実行に成功したら終わり。
				for (auto it = function_map_.rbegin(); it!= function_map_.rend(); ++it) {
					auto fit = it->second->find(func_name);
					if (fit = it->second->end()) {
						continue;
					}
					else {
						if (SearchFunction(fit->second, params...)) { return true; }
						else { continue; }
					}
				}
				debug::SystemLog::instance().LogWarning(std::string("関数が存在しません。") + func_name + +"(" + mp_utiliey::ConvertParametersToString(params...) + ")", __FUNCTION__);
				return false;
			}
		protected:
			/*必ずReflectionを使うクラスのコンストラクタ全てで実行する*/
			template<class C>
			void SetUpRefrection(C* this_pointer) {
				static ClassInformationSetupper re(this_pointer); //クラスの登録処理。静的関数なので初回しか実行されない。
				auto it = type_function_map_.find(typeid(C));
				assert(it != type_function_map_.end()); //クラス登録は済んでいるはずなので見つからないのはおかしい。
				if (function_map_.size() > 0 && (*(function_map_.end() - 1)) == &(it->second)) { //関数マップの最後尾が、新たに登録しようとしているものと一致した
					debug::SystemLog::instance().LogWarning(std::string("リフレクションのセットアップが重複して行われました。(ClassType = ") + typeid(C).name() + ", Address = " + boost::lexical_cast<std::string>(this_pointer) + ")", __FUNCTION__);
					return;
				}
				function_map_.push_back(&it->second);
			}			

		private:
			struct FunctionHolder {
				std::unordered_map<std::type_index, boost::any> functions;
			};
			using FunctionMap = std::unordered_map<std::string, FunctionHolder>;
		protected:
			class RefrectionInformationResistInterface final {
			public:
				RefrectionInformationResistInterface(FunctionMap& func_map,const std::function<bool(Reflection*)>& type_checker) :func_map_(func_map),type_checker_(type_checker) {};
				template<typename ClassType, typename ... TArgs>
				void ResistFunction(const std::string name, const std::function<void(ClassType*, TArgs ...)>& func) {
					//有効なタイプか確認
					if (!type_checker_(&ClassType())) {
						throw std::logic_error(std::string("クラスのタイプが不正です。") + name + "(" + mp_utiliey::ConvertParameterTypesToString<TArgs...>() + ")");
					}
					//登録する関数を作成
					std::function<void(Reflection* this_base, TArgs...)> resist_func = [func](Reflection* this_base, TArgs... params) {func(static_cast<ClassType*>(this_base), params...)};
					//関数登録時に用いるタイプデータ
					using namespace mp_utiliey;
					using resist_func_type = typename ExpandParameterList < typename ConvertParameterList<typename List<Reflection*, TArgs...>::list>::param_list>::type;
					std::type_info resist_func_type_info = typeid(resist_func);
					/*void(Refrection*,Params...)の形式で関数を登録*/
					func_map_[name].functions.emplace(resist_func_type_info, resist_func);
				}
				void ResistToClassFactory(const std::string&){

				}
			private:
				Reflection::FunctionMap& func_map_;
				std::function<bool(Reflection*)> type_checker_;
			};
		private:

			virtual void ResistRefrectionInformation(RefrectionInformationResistInterface&&) = 0;

			std::vector<FunctionMap*> function_map_;
			static std::unordered_map<std::type_index, FunctionMap> type_function_map_;

			template<typename ... Params>
			bool SearchFunction(const FunctionHolder& fh, Params ... params) {
				using func_type = std::function<void(Params...)>;
				std::type_info func_type_info = typeid(func_type);
				auto it = fh.functions.find(func_type_info);
				if (it == fh.functions.end()) {
					return false;
				}
				else {
					CallFunction(it->second, this, params...);
				}
			}

			template<typename ... TArgs>
			void CallFunction(const boost::any& func,TArgs ... params) {
				using namespace mp_utiliey;
				using func_type_temp = typename ExpandParameterList < typename ConvertParameterList<typename List<TArgs...>::list>::param_list>::type;
				try {
					func_type_temp::Call(boost::any_cast<typename func_type_temp::func_type<void>>(func), params...);
				}
				catch (boost::bad_any_cast&) {
					assert(false); //関数型は登録時と検索時で一致しているはずなので、ここで不一致なのはおかしい。
				}
			}

			class ClassInformationSetupper final{
			public:
				template<class C>
				explicit ClassInformationSetupper(C* c) {
					Reflection* this_r = c;
					auto it = this_r->type_function_map_.emplace(typeid(C), FunctionMap()).first; //クラス情報を登録
					//ここが呼び出される時点でCクラスが構築されているはずなので大丈夫？
					this_r->ResistRefrectionInformation(RefrectionInformationResistInterface(it->second, [](Reflection* r) {return dynamic_cast<C*>(r) ? true : false; }));
				}
				~ClassInformationSetupper() = default;
			};
		};
	}
}