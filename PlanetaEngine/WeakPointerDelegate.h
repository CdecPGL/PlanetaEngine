#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"

namespace planeta_engine {
	namespace utility {
		//イベント関数所有クラスインターフェイス
		template<typename EventArgType>
		class IEventFunctionHolder {
		public:
			using ParamType = typename boost::call_traits<EventArgType>::param_type;
			virtual ~IEventFunctionHolder() = default;
			virtual bool Call(ParamType) = 0;
		};

		template<>
		class IEventFunctionHolder<void> {
		public:
			virtual ~IEventFunctionHolder() = default;
			virtual bool Call() = 0;
		};
		//イベントメンバ関数所有クラス
		template<typename EventArgType,class C>
		class EventMenberFunctionHolder final: public IEventFunctionHolder<EventArgType>{
		public:
			using FunctionType = void(C::*)(typename IEventFunctionHolder<EventArgType>::ParamType);
			EventMenberFunctionHolder(const utility::WeakPointer<C>& wp, FunctionType f):w_ptr_(wp),func_(f) {
			}
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
				if (w_ptr_) {
					((*w_ptr_).*func_)(e);
					return true;
				}
				else {
					return false;
				}
			}
		private:
			utility::WeakPointer<C> w_ptr_;
			FunctionType func_;
		};

		template<class C>
		class EventMenberFunctionHolder<void,C> final : public IEventFunctionHolder<void> {
		public:
			using FunctionType = void(C::*)();
			EventMenberFunctionHolder(const utility::WeakPointer<C>& wp, FunctionType f) :w_ptr_(wp), func_(f) {
			}
			bool Call()override {
				if (w_ptr_) {
					((*w_ptr_).*func_)();
					return true;
				}
				else {
					return false;
				}
			}
		private:
			utility::WeakPointer<C> w_ptr_;
			FunctionType func_;
		};

		//イベント通常関数保有クラス
		template<typename EventArgType>
		class EventNormalFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			using FunctionType = std::function<void(typename IEventFunctionHolder<EventArgType>::ParamType)>;
			explicit EventNormalFunctionHolder(const FunctionType& f) :func_(f) {}
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
				func_(e);
				return true;
			}
		private:
			FunctionType func_;
		};
		template<>
		class EventNormalFunctionHolder<void> final : public IEventFunctionHolder<void> {
		public:
			using FunctionType = std::function<void()>;
			explicit EventNormalFunctionHolder(const FunctionType& f) :func_(f) {}
			bool Call()override {
				func_();
				return true;
			}
		private:
			FunctionType func_;
		};
		//ダミー関数保有クラス
		template<typename EventArgType>
		class EventDumyFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
				return false;
			}
		};
		template<>
		class EventDumyFunctionHolder<void> final : public IEventFunctionHolder<void> {
		public:
			bool Call()override {
				return false;
			}
		};
		//WeakPointerDelegateへの接続クラスインターフェス
		class IWeakPointerDelegateConnecter {
		public:
			virtual ~IWeakPointerDelegateConnecter() = default;
			virtual void Remove() = 0;
		};
		//WeakPointerDelegateへの接続クラス
		template<typename EventArgType>
		class WeakPointerDelegateConnecter final : public IWeakPointerDelegateConnecter{
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventFunctionHolder<EventArgType>>>;
			WeakPointerDelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list,size_t id) :handler_list_(handler_list),id_(id) {}
			/*登録したデリゲートから削除*/
			void Remove()override {  
				if (!handler_list_.expired()) {
					auto handler_list_ptr = handler_list_.lock();
					auto it = handler_list_ptr->find(id_);
					if (it != handler_list_ptr->end()) {
						//対象IDに関数に必ずfalseを返すダミーを登録することで、次回イベント発生時にリストから取り除いてもらう。こうすることでイベント発生時のハンドラ呼び出しループ中にこの関数が呼び出されても安全。
						it->second = std::make_unique<EventDumyFunctionHolder<EventArgType>>();
					}
				}
			}
		private:
			std::weak_ptr<HandlerListType> handler_list_;
			size_t id_;
		};
		/**
		* @brief WeakPointerDelegateへの接続保持クラス
		*/
		class WeakPointerDelegateConnection final{
		public:
			explicit WeakPointerDelegateConnection(const std::shared_ptr<IWeakPointerDelegateConnecter>& cp) :connector_(cp) {}
			/**
			* @brief デリゲートから削除する
			*/
			void Remove() { connector_->Remove(); }
		private:
			std::shared_ptr<IWeakPointerDelegateConnecter> connector_;
		};
		/**
		* @brief WeakPointerデリゲート
		*/
		template<typename EventArgType = void>
		class WeakPointerDelegate final{
		public:
			WeakPointerDelegate():handlers_(std::make_shared<WeakPointerDelegateConnecter<EventArgType>::HandlerListType>()){}
			void operator()(typename IEventFunctionHolder<EventArgType>::ParamType e) {
				for (auto it = handlers_->begin(); it != handlers_->end();) {
					if (it->second->Call(e)) {
						++it;
					}
					else {
						handlers_->erase(it++);
					}
				}
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (c) オブジェクトのWeakPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(typename IEventFunctionHolder<EventArgType>::ParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventMenberFunctionHolder<EventArgType, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			WeakPointerDelegateConnection Add(const std::function<void(typename IEventFunctionHolder<EventArgType>::ParamType)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventNormalFunctionHolder<EventArgType>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<EventArgType>::HandlerListType> handlers_;
		};

		template<>
		class WeakPointerDelegate<void> final {
		public:
			WeakPointerDelegate() :handlers_(std::make_shared<WeakPointerDelegateConnecter<void>::HandlerListType>()) {}
			void operator()() {
				for (auto it = handlers_->begin(); it != handlers_->end();) {
					if (it->second->Call()) {
						++it;
					}
					else {
						handlers_->erase(it++);
					}
				}
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (c) オブジェクトのWeakPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventMenberFunctionHolder<void>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			WeakPointerDelegateConnection Add(const std::function<void(void)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventNormalFunctionHolder<void>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<void>::HandlerListType> handlers_;
		};
	}
}
