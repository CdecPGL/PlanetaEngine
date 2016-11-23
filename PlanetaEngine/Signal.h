#pragma once

#include "boost/signals2/signal.hpp"

#include "WeakPointer.h"

namespace planeta {
	/*! シグナルへの接続クラス*/
	class SignalConnection final{
	public:
		/*! デフォルトコンストラクタ*/
		SignalConnection() = default;
		/*! コピーコンストラクタ*/
		SignalConnection(const SignalConnection& obj) :boost_sig2_connection_(obj.boost_sig2_connection_) {}
		/*! ムーブコンストラクタ*/
		SignalConnection(SignalConnection&& obj) :boost_sig2_connection_(std::move(obj.boost_sig2_connection_)) {}
		/*! コンストラクタ*/
		explicit SignalConnection(boost::signals2::connection&& bsc) :boost_sig2_connection_(std::move(bsc)) {}
		/*! 代入演算子*/
		SignalConnection& operator=(const SignalConnection& obj) {
			boost_sig2_connection_ = obj.boost_sig2_connection_;
			return *this;
		}
		/*! ムーブ演算子*/
		SignalConnection& operator=(SignalConnection&& obj) {
			boost_sig2_connection_ = std::move(obj.boost_sig2_connection_);
			return *this;
		}
		/*! シグナルへの接続を切断する*/
		void Disconnect()const {
			boost_sig2_connection_.disconnect();
		}
		/*! シグナルへ接続されているかを取得*/
		bool is_connected()const {
			return boost_sig2_connection_.connected();
		}
	private:
		boost::signals2::connection boost_sig2_connection_;
	};

	/*! シグナルクラス*/
	template<typename SigType>
	class Signal final: private boost::signals2::signal<SigType> {
	public:
		/*! シグナル関数の引数型*/
		using ArgType = typename boost::function_traits<SigType>::arg1_type;
		/*! シグナル関数の戻り値型*/
		using RetType = typename boost::function_traits<SigType>::result_type;
		/*! 通常関数を接続*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return SignalConnection{ std::move(connect(func)) };
		}
		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func, boost::placeholders::_1).track_foreign(ins))) };
		}
		/*! 通常関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func, boost::placeholders::_1).track_foreign(ins.get_shared()))) };
		}
		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, RetType(InsType::*m_func)(ArgType)) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins))) };
		}
		/*! メンバ関数をWeakPointerを指定して接続
		
			インスタンスの型は、メンバ関数のクラス型と同じかその子クラスである必要がある。
		*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, RetType(InsType::*m_func)(ArgType)) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins.get_shared()))) };
		}
		/*! 関数呼び出し*/
		auto operator()(const ArgType& arg) {
			return signal::operator ()(arg);
		}
	};

	/*! シグナルクラス(void特殊化)*/
	template<typename RetType>
	class Signal<RetType(void)> final: private boost::signals2::signal<RetType(void)> {
	public:
		/*! 通常関数を接続*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return SignalConnection{ std::move(connect(func)) };
		}
		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func).track_foreign(ins))) };
		}
		/*! 通常関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func).track_foreign(ins.get_shared()))) };
		}
		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, RetType(InsType::*m_func)()) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get()).track_foreign(ins))) };
		}
		/*! メンバ関数をWeakPointerを指定して接続*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, RetType(InsType::*m_func)()) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get()).track_foreign(ins.get_shared()))) };
		}
		/*! 関数呼び出し*/
		auto operator()() {
			return signal::operator ()();
		}
	};
}
