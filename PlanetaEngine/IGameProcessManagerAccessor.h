#pragma once

#include <string>
#include <functional>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "boost/preprocessor.hpp"

/**
* @brief 特定型の要素をWeakPointerで取得する公開純粋仮想テンプレート関数を疑似的に実装するためのマクロ。
* 公開純粋仮想関数として"template<class C> utility::WeakPointer<C> func_name()const"が作成される。
* 子クラスは"virtual utility::WeakPointer<element_base_type> func_name(std::function<bool(const std::shared_ptr<element_base_type>&)> type_checker)const = 0"をprivateでオーバーライドしなければならない。
* @param (func_name) 実装する関数名
* @param (element_base_type) 要素の基底型
*/
#define PUBLIC_PURE_VIRTUAL_TEMPLATE_WEAKPOINTER_ELEMENT_GETTER_BY_TYPE(element_base_type,func_name)\
private:\
virtual utility::WeakPointer<element_base_type> func_name(std::function<bool(const std::shared_ptr<element_base_type>&)> type_checker)const = 0;\
public:\
template<class C>\
utility::WeakPointer<C> func_name()const {\
	return utility::static_weak_pointer_cast<C>(func_name(&utility::CheckSharedPointerTypeOfBase<C, element_base_type>));\
}

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	};
	namespace game {
		class GameProcess;
		class IGameProcessManagerAccessor {
		public:
			virtual ~IGameProcessManagerAccessor() = 0 {};
			/*ゲームプロセスを作成する*/
		private:virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority) = 0;
		public:
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga, int id) {return std::make_shared<C>(ga, id); }, priority));
			}
			/*ゲームプロセスをIDから取得する*/
			virtual utility::WeakPointer<GameProcess> GetGameProcess(int id) = 0;
			/*ゲームプロセスをIDで削除する*/
			virtual bool RemoveGameProcess(int id) = 0;
			/*システムプロセスを型から取得する*/
			PUBLIC_PURE_VIRTUAL_TEMPLATE_WEAKPOINTER_ELEMENT_GETTER_BY_TYPE(GameProcess, GetSystemProcess);
			/*システムプロセスを名前から取得する*/
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) = 0;
		};
	}
}
