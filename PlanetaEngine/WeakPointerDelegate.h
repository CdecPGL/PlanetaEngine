#pragma once

#include "WeakPointer.h"
#include <unordered_map>
#include <memory>
#include <functional>

namespace planeta_engine {
	namespace utility {
		//イベント関数所有クラスインターフェイス
		template<typename EventArgType>
		class IEventFunctionHolder {
		public:
			virtual ~IEventFunctionHolder() = default;
			virtual bool Call(const EventArgType&) = 0;
		};
		//イベントメンバ関数所有クラス
		template<typename EventArgType,class C>
		class EventMenberFunctionHolder final: public IEventFunctionHolder<EventArgType>{
		public:
			using FunctionType = void(C::*)(const EventArgType&);
			EventMenberFunctionHolder(const utility::WeakPointer<C>& wp, FunctionType f):w_ptr_(wp),func_(f) {
			}
			bool Call(const EventArgType& e)override {
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
		//イベント通常関数保有クラス
		template<typename EventArgType>
		class EventNormalFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			using FunctionType = std::function<void(const EventArgType&)>;
			explicit EventNormalFunctionHolder(const FunctionType& f) :func_(f) {}
			bool Call(const EventArgType& e)override {
				func_(e);
				return true;
			}
		private:
			FunctionType func_;
		};
		//ダミー関数保有クラス
		template<typename EventArgType>
		class EventDumyFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			bool Call(const EventArgType&)override {
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
		//WeakPointerDelegateへの接続保持クラス
		class WeakPointerDelegateConnection final{
		public:
			explicit WeakPointerDelegateConnection(const std::shared_ptr<IWeakPointerDelegateConnecter>& cp) :connector_(cp) {}
			void Remove() { connector_->Remove(); }
		private:
			std::shared_ptr<IWeakPointerDelegateConnecter> connector_;
		};
		//WeakPointerデリゲート
		template<typename EventArgType>
		class WeakPointerDelegate final{
		public:
			WeakPointerDelegate():handlers_(std::make_shared<WeakPointerDelegateConnecter<EventArgType>::HandlerListType>()){}
			void operator()(const EventArgType& e) {
				for (auto it = handlers_->begin(); it != handlers_->end();) {
					if (it->second->Call(e)) {
						++it;
					}
					else {
						handlers_->erase(it++);
					}
				}
			}
			/*デリケートにメンバ関数を登録(オブジェクトのWeakPointer、メンバ関数ポインタ)*/
			template<class C>
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(const EventArgType&)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventMenberFunctionHolder<EventArgType, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
			/*デリゲートに通常関数を登録*/
			WeakPointerDelegateConnection Add(const std::function<void(const EventArgType&)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventNormalFunctionHolder<EventArgType>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<EventArgType>::HandlerListType> handlers_;
		};
	}
}
