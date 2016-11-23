#pragma once

#include "boost/signals2/signal.hpp"

#include "WeakPointer.h"

namespace planeta {
	/*! �V�O�i���ւ̐ڑ��N���X*/
	class SignalConnection final{
	public:
		explicit SignalConnection(boost::signals2::connection&& bsc) :boost_sig2_connection_(std::move(bsc)) {}
		/*! �V�O�i���ւ̐ڑ���ؒf����*/
		void Disconnect() {
			boost_sig2_connection_.disconnect();
		}
	private:
		boost::signals2::connection boost_sig2_connection_;
	};

	/*! �V�O�i���N���X*/
	template<typename SigType>
	class Signal final: private boost::signals2::signal<SigType> {
	public:
		/*! �V�O�i���֐��̈����^*/
		using ArgType = typename boost::function_traits<SigType>::arg1_type;
		/*! �ʏ�֐���ڑ�*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return connect(func);
		}
		/*! �ʏ�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, boost::placeholders::_1).track_foreign(ins));
		}
		/*! �ʏ�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, boost::placeholders::_1).track_foreign(ins.get_shared()));
		}
		/*! �����o�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get(), boost::placeholders::_1).track_foreign(ins));
		}
		/*! �����o�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get(), boost::placeholders::_1).track_foreign(ins.get_shared()));
		}
		/*! �֐��Ăяo��*/
		auto operator()(const ArgType& arg) {
			return signal::operator ()(arg);
		}
	};

	/*! �V�O�i���N���X(void���ꉻ)*/
	template<typename RetType>
	class Signal<RetType(void)> final: private boost::signals2::signal<RetType(void)> {
		/*! �ʏ�֐���ڑ�*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return connect(func);
		}
		/*! �ʏ�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func).track_foreign(ins));
		}
		/*! �ʏ�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func).track_foreign(ins.get_shared()));
		}
		/*! �����o�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get()).track_foreign(ins));
		}
		/*! �����o�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return connect(slot_type(func, ins.get()).track_foreign(ins.get_shared()));
		}
		/*! �֐��Ăяo��*/
		auto operator()() {
			return signal::operator ()();
		}
	};
}
