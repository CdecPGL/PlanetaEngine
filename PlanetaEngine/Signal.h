#pragma once

#include "boost/signals2/signal.hpp"

#include "WeakPointer.h"

namespace planeta {
	/*! シグナルへの接続クラス*/
	class SignalConnection final{
	public:
		explicit SignalConnection(boost::signals2::connection&& bsc) :boost_sig2_connection_(std::move(bsc)) {}
		/*! シグナルへの接続を切断する*/
		void Disconnect() {
			boost_sig2_connection_.disconnect();
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
		/*! 通常関数を接続*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return connect(func);
		}
		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, boost::placeholders::_1).track_foreign(ins));
		}
		/*! 通常関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, boost::placeholders::_1).track_foreign(ins.get_shared()));
		}
		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get(), boost::placeholders::_1).track_foreign(ins));
		}
		/*! メンバ関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get(), boost::placeholders::_1).track_foreign(ins.get_shared()));
		}
		/*! 関数呼び出し*/
		auto operator()(const ArgType& arg) {
			return signal::operator ()(arg);
		}
	};

	/*! シグナルクラス(void特殊化)*/
	template<typename RetType>
	class Signal<RetType(void)> final: private boost::signals2::signal<RetType(void)> {
		/*! 通常関数を接続*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return connect(func);
		}
		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func).track_foreign(ins));
		}
		/*! 通常関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func).track_foreign(ins.get_shared()));
		}
		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get()).track_foreign(ins));
		}
		/*! メンバ関数をWeakPointerを指定して接続*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get()).track_foreign(ins.get_shared()));
		}
		/*! 関数呼び出し*/
		auto operator()() {
			return signal::operator ()();
		}
	};
}
