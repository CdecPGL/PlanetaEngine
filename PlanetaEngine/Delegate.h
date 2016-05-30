#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"

namespace planeta_engine {
	namespace utility {
		//イベントハンドラ所有クラスインターフェイス(内部クラス)
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
		//WeakPointerメンバ関数イベントハンドラ所有クラス(内部クラス)
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

		//メンバ関数イベントハンドラ所有クラス(内部クラス)
		template<typename EventArgType, class C>
		class MenberFunctionEventHandlerHolder final : public IEventHandlerHolder<EventArgType> {
		public:
			using FunctionType = void(C::*)(ParamType);
			MenberFunctionEventHandlerHolder(C* p, FunctionType f)noexcept :ptr_(p), func_(f) {}
			~MenberFunctionEventHandlerHolder()noexcept = default;
			bool Call(ParamType e)override {
				(ptr_->*func_)(e);
				return true;
			}
		private:
			C* ptr_;
			FunctionType func_;
		};

		template<class C>
		class MenberFunctionEventHandlerHolder<void, C> final : public IEventHandlerHolder<void> {
		public:
			using FunctionType = void(C::*)();
			MenberFunctionEventHandlerHolder(C* p, FunctionType f)noexcept :ptr_(p), func_(f) {}
			~MenberFunctionEventHandlerHolder()noexcept = default;
			bool Call()override {
				(ptr_->*func_)();
				return true;
			}
		private:
			C* ptr_;
			FunctionType func_;
		};

		//通常関数イベントハンドラ保有クラス(内部クラス)
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
		//破棄イベントハンドラ保有クラス(内部クラス)
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
		//Delegateへの接続クラスインターフェス(内部クラス)
		class IDelegateConnecter {
		public:
			IDelegateConnecter()noexcept = default;
			virtual ~IDelegateConnecter()noexcept = default;
			virtual void Remove() = 0;
		};
		//Delegateへの接続クラス(内部クラス)
		template<typename EventArgType>
		class DelegateConnecter final : public IDelegateConnecter {
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventHandlerHolder<EventArgType>>>;
			DelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list, size_t id)noexcept :handler_list_(handler_list), id_(id) {}
			~DelegateConnecter()noexcept = default;
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
		* @brief Delegateへの接続保持クラス(公開クラス)
		*/
		class DelegateConnection final {
		public:
			DelegateConnection()noexcept = default;
			DelegateConnection(const DelegateConnection& wpdc)noexcept :connector_(wpdc.connector_) {}
			DelegateConnection(DelegateConnection&& wpdc)noexcept : connector_(std::move(wpdc.connector_)) {}
			explicit DelegateConnection(const std::shared_ptr<IDelegateConnecter>& cp)noexcept : connector_(cp) {}
			explicit DelegateConnection(std::shared_ptr<IDelegateConnecter>&& cp)noexcept : connector_(std::move(cp)) {}
			DelegateConnection& operator =(const DelegateConnection& wpdc)noexcept { connector_ = wpdc.connector_; return *this; }
			DelegateConnection& operator =(DelegateConnection&& wpdc)noexcept { connector_ = std::move(wpdc.connector_); return *this; }
			/**
			* @brief デリゲートから削除する
			*/
			void Remove() { if (connector_) { connector_->Remove(); connector_.reset(); } }
		private:
			std::shared_ptr<IDelegateConnecter> connector_;
		};
		/**
		* @brief デリゲート(公開クラス)
		*/
		template<typename EventArgType = void>
		class Delegate final {
		public:
			using HandlerParamType = typename IEventHandlerHolder<EventArgType>::ParamType;
			Delegate()noexcept:handlers_(std::make_shared<DelegateConnecter<EventArgType>::HandlerListType>()) {}
			Delegate(const Delegate&) = delete;
			~Delegate()noexcept = default;
			Delegate& operator=(const Delegate&) = delete;

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
			* @brief デリケートにWeakPointerメンバ関数を登録する
			* @param (c) オブジェクトのWeakPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			DelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (c) オブジェクトのWeakPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			DelegateConnection Add(C* c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<MenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief デリケートに通常関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			DelegateConnection Add(const std::function<void(HandlerParamType)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<NormalFunctionEventHandlerHolder<EventArgType>>(func));
				return DelegateConnection(std::make_shared<DelegateConnecter<EventArgType>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename DelegateConnecter<EventArgType>::HandlerListType> handlers_;
		};

		template<>
		class Delegate<void> final {
		public:
			using HandlerParamType = void;
			Delegate()noexcept : handlers_(std::make_shared<DelegateConnecter<void>::HandlerListType>()) {}
			Delegate(const Delegate&) = delete;
			~Delegate()noexcept = default;
			Delegate& operator=(const Delegate&) = delete;

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
			* @brief デリケートにWeakPointerメンバ関数を登録する
			* @param (c) オブジェクトのWeakPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			DelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<void, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief デリケートにメンバ関数を登録する
			* @param (c) オブジェクトのPointer
			* @param (f) メンバ関数ポインタ
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			template<class C>
			DelegateConnection Add(C* c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<MenberFunctionEventHandlerHolder<void, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief デリケートに通常関数を登録する
			* @param (func) 登録したい関数
			* @return ハンドラの削除に使うデリゲート接続クラス
			*/
			DelegateConnection Add(const std::function<void(void)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<NormalFunctionEventHandlerHolder<void>>(func));
				return DelegateConnection(std::make_shared<DelegateConnecter<void>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename DelegateConnecter<void>::HandlerListType> handlers_;
		};

		/*デリゲートハンドラ追加クラス(公開クラス)*/
		template<typename EventArgType = void>
		class DelegateHandlerAdder final{
		public:
			DelegateHandlerAdder(std::function<DelegateConnection(Delegate<EventArgType>&)>&& adder) :delegate_handle_adder_(std::move(adder)) {}
			DelegateConnection operator()(Delegate<EventArgType>& dlgt) { return delegate_handle_adder_(dlgt); }
		private:
			std::function<DelegateConnection(Delegate<EventArgType>&)> delegate_handle_adder_;
		};

		/*WeakMenberデリゲート追加クラス作成(公開関数)*/
		template<typename EventArgType,class C>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(const WeakPointer<C>& c, typename void(C::*f)(EventArgType)) {
			return std::move(DelegateHandlerAdder<EventArgType>([c, f](Delegate<EventArgType>& dlgt) {return dlgt.Add(c, f); }));
		}
		template<class C>
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(const WeakPointer<C>& c, typename void(C::*f)()) {
			return std::move(DelegateHandlerAdder<void>([c, f](Delegate<void>& dlgt) {return dlgt.Add(c, f); }));
		}
		/*MemberFunctionデリゲート追加クラス作成(公開関数)*/
		template<typename EventArgType, class C>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(C* c, typename void(C::*f)(EventArgType)) {
			return std::move(DelegateHandlerAdder<EventArgType>([c, f](Delegate<EventArgType>& dlgt) {return dlgt.Add(c, f); }));
		}
		template<class C>
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(C* c, typename void(C::*f)()) {
			return std::move(DelegateHandlerAdder<void>([c, f](Delegate<void>& dlgt) {return dlgt.Add(c, f); }));
		}
		/*デリゲート追加クラス作成。EventArgがvoidの時はテンプレート引数なし、それ以外は指定して利用する。(公開関数)*/
		template<typename EventArgType>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(const std::function<void(EventArgType)>& func) {
			return std::move(DelegateHandlerAdder<EventArgType>([func](Delegate<EventArgType>& dlgt) {return dlgt.Add(func); }));
		}
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(const std::function<void()>& func) {
			return std::move(DelegateHandlerAdder<void>([func](Delegate<void>& dlgt) {return dlgt.Add(func); }));
		}
	}
}
