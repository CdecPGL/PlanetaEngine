#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"
#include "PEAssert.h"

namespace planeta_engine {
	namespace utility {
		//イベントハンドラ所有クラスインターフェイス
		template<typename EventArgType>
		class IEventHandlerHolder {
		public:
			using ParamType = typename boost::call_traits<EventArgType>::param_type;
			IEventHandlerHolder()noexcept = default;
			virtual ~IEventHandlerHolder()noexcept = default;
			virtual bool Call(ParamType) = 0;
		};

		template<>
		class IEventHandlerHolder<void> {
		public:
			IEventHandlerHolder()noexcept = default;
			virtual ~IEventHandlerHolder()noexcept = default;
			virtual bool Call() = 0;
		};
		//WeakPointerメンバ関数イベントハンドラ所有クラス
		template<typename EventArgType, class C>
		class WeakPointerMenberFunctionEventHandlerHolder final : public IEventHandlerHolder<EventArgType> {
		public:
			using FunctionType = void(C::*)(ParamType);
			WeakPointerMenberFunctionEventHandlerHolder(const utility::WeakPointer<C>& wp, FunctionType f)noexcept :w_ptr_(wp), func_(f) {}
			~WeakPointerMenberFunctionEventHandlerHolder()noexcept = default;
			bool Call(ParamType e)override {
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
		class WeakPointerMenberFunctionEventHandlerHolder<void, C> final : public IEventHandlerHolder<void> {
		public:
			using FunctionType = void(C::*)();
			WeakPointerMenberFunctionEventHandlerHolder(const utility::WeakPointer<C>& wp, FunctionType f)noexcept :w_ptr_(wp), func_(f) {}
			~WeakPointerMenberFunctionEventHandlerHolder()noexcept = default;
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

		//通常関数イベントハンドラ保有クラス
		template<typename EventArgType>
		class NormalFunctionEventHandlerHolder final : public IEventHandlerHolder<EventArgType> {
		public:
			using FunctionType = std::function<void(ParamType)>;
			explicit NormalFunctionEventHandlerHolder(const FunctionType& f)noexcept :func_(f) {}
			~NormalFunctionEventHandlerHolder()noexcept = default;
			bool Call(ParamType e)override {
				func_(e);
				return true;
			}
		private:
			FunctionType func_;
		};
		template<>
		class NormalFunctionEventHandlerHolder<void> final : public IEventHandlerHolder<void> {
		public:
			using FunctionType = std::function<void()>;
			explicit NormalFunctionEventHandlerHolder(const FunctionType& f)noexcept :func_(f) {}
			~NormalFunctionEventHandlerHolder()noexcept = default;
			bool Call()override {
				func_();
				return true;
			}
		private:
			FunctionType func_;
		};
		//破棄イベントハンドラ保有クラス
		template<typename EventArgType>
		class GarbageEventHandlerHolder final : public IEventHandlerHolder<EventArgType> {
		public:
			GarbageEventHandlerHolder(std::unique_ptr<IEventHandlerHolder<EventArgType>>&& garbage)noexcept : garbage_(std::move(garbage)) {}
			~GarbageEventHandlerHolder()noexcept = default;
			bool Call(ParamType)override { return false; }
		private:
			std::unique_ptr<IEventHandlerHolder<EventArgType>> garbage_;
		};

		template<>
		class GarbageEventHandlerHolder<void> final : public IEventHandlerHolder<void> {
		public:
			GarbageEventHandlerHolder(std::unique_ptr<IEventHandlerHolder<void>>&& garbage)noexcept : garbage_(std::move(garbage)) {}
			~GarbageEventHandlerHolder()noexcept = default;
			bool Call()override { return false; }
		private:
			std::unique_ptr<IEventHandlerHolder<void>> garbage_;
		};
		//WeakPointerDelegateへの接続クラスインターフェス
		class IWeakPointerDelegateConnecter {
		public:
			IWeakPointerDelegateConnecter()noexcept = default;
			virtual ~IWeakPointerDelegateConnecter()noexcept = default;
			virtual void Remove() = 0;
		};
		//WeakPointerDelegateへの接続クラス
		template<typename EventArgType>
		class WeakPointerDelegateConnecter final : public IWeakPointerDelegateConnecter {
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventHandlerHolder<EventArgType>>>;
			WeakPointerDelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list, size_t id)noexcept :handler_list_(handler_list), id_(id) {}
			~WeakPointerDelegateConnecter()noexcept = default;
			/*登録したデリゲートから削除*/
			void Remove()override {
				if (!handler_list_.expired()) {
					auto handler_list_ptr = handler_list_.lock();
					auto it = handler_list_ptr->find(id_);
					assert(it != handler_list_ptr->end()); //見つからないのはおかしい。
					//対象IDに関数に必ずfalseを返す破棄ハンドラ保持クラスを登録することで、次回イベント発生時にリストから取り除いてもらう。こうすることでイベント発生時のハンドラ呼び出しループ中にこの関数が呼び出されても安全。
					//ここでハンドラを破棄したとき、ハンドラの所有者が自分だけだったら(ラムダ式をハンドラに登録した場合。Weakポインタとメンバ関数ポインタのセットを登録した場合は他に所有者がいるということなのでおそらくあてはまらない。)ハンドラはメモリ上から削除される。
					//しかしこのRemove()をハンドラ自身が呼んでいた場合はここRemove()を抜けた後にハンドラに戻るため、メモリ上から削除されたハンドラを実行してしまうことになり動作が未確定になる。
					//新たに登録する破棄ハンドラ保持クラスに破棄対象のハンドラの所有権を移動することで、ここではメモリ上からの削除は行わずこの問題を回避する。
					//破棄対象ハンドラは次回イベント発生時に破棄ハンドラ保持クラスと一緒に破棄されることになる。
					it->second = std::make_unique<GarbageEventHandlerHolder<EventArgType>>(std::move(it->second));
					handler_list_.reset(); //削除が完了したら、リストへの参照は切っておく
				}
			}
		private:
			std::weak_ptr<HandlerListType> handler_list_;
			size_t id_;
		};
		/**
		* @brief WeakPointerDelegateへの接続保持クラス
		*/
		class WeakPointerDelegateConnection final {
		public:
			WeakPointerDelegateConnection()noexcept = default;
			WeakPointerDelegateConnection(const WeakPointerDelegateConnection& wpdc)noexcept :connector_(wpdc.connector_) {}
			WeakPointerDelegateConnection(WeakPointerDelegateConnection&& wpdc)noexcept : connector_(std::move(wpdc.connector_)) {}
			explicit WeakPointerDelegateConnection(const std::shared_ptr<IWeakPointerDelegateConnecter>& cp)noexcept : connector_(cp) {}
			explicit WeakPointerDelegateConnection(std::shared_ptr<IWeakPointerDelegateConnecter>&& cp)noexcept : connector_(std::move(cp)) {}
			WeakPointerDelegateConnection& operator =(const WeakPointerDelegateConnection& wpdc)noexcept { connector_ = wpdc.connector_; return *this; }
			WeakPointerDelegateConnection& operator =(WeakPointerDelegateConnection&& wpdc)noexcept { connector_ = std::move(wpdc.connector_); return *this; }
			/**
			* @brief デリゲートから削除する
			*/
			void Remove() { if (connector_) { connector_->Remove(); connector_.reset(); } }
		private:
			std::shared_ptr<IWeakPointerDelegateConnecter> connector_;
		};
		/**
		* @brief WeakPointerデリゲート
		*/
		template<typename EventArgType = void>
		class WeakPointerDelegate final {
		public:
			using HandlerParamType = typename IEventHandlerHolder<EventArgType>::ParamType;
			WeakPointerDelegate()noexcept:handlers_(std::make_shared<WeakPointerDelegateConnecter<EventArgType>::HandlerListType>()) {}
			~WeakPointerDelegate()noexcept = default;

			void operator()(HandlerParamType e) {
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
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			WeakPointerDelegateConnection Add(const std::function<void(HandlerParamType)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<NormalFunctionEventHandlerHolder<EventArgType>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<EventArgType>::HandlerListType> handlers_;
		};

		template<>
		class WeakPointerDelegate<void> final {
		public:
			WeakPointerDelegate()noexcept : handlers_(std::make_shared<WeakPointerDelegateConnecter<void>::HandlerListType>()) {}
			~WeakPointerDelegate()noexcept = default;

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
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<void, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			WeakPointerDelegateConnection Add(const std::function<void(void)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<NormalFunctionEventHandlerHolder<void>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<void>::HandlerListType> handlers_;
		};
	}
}
