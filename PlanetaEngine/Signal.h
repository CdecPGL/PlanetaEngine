#pragma once

#include "boost/signals2/signal.hpp"

#include "WeakPointer.h"

namespace planeta {
	/*! �V�O�i���ւ̐ڑ��N���X*/
	class SignalConnection final{
	public:
		/*! �f�t�H���g�R���X�g���N�^*/
		SignalConnection() = default;
		/*! �R�s�[�R���X�g���N�^*/
		SignalConnection(const SignalConnection& obj) :boost_sig2_connection_(obj.boost_sig2_connection_) {}
		/*! ���[�u�R���X�g���N�^*/
		SignalConnection(SignalConnection&& obj) :boost_sig2_connection_(std::move(obj.boost_sig2_connection_)) {}
		/*! �R���X�g���N�^*/
		explicit SignalConnection(boost::signals2::connection&& bsc) :boost_sig2_connection_(std::move(bsc)) {}
		/*! ������Z�q*/
		SignalConnection& operator=(const SignalConnection& obj) {
			boost_sig2_connection_ = obj.boost_sig2_connection_;
			return *this;
		}
		/*! ���[�u���Z�q*/
		SignalConnection& operator=(SignalConnection&& obj) {
			boost_sig2_connection_ = std::move(obj.boost_sig2_connection_);
			return *this;
		}
		/*! �V�O�i���ւ̐ڑ���ؒf����*/
		void Disconnect()const {
			boost_sig2_connection_.disconnect();
		}
		/*! �V�O�i���֐ڑ�����Ă��邩���擾*/
		bool is_connected()const {
			return boost_sig2_connection_.connected();
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
		/*! �V�O�i���֐��̖߂�l�^*/
		using RetType = typename boost::function_traits<SigType>::result_type;
		/*! �ʏ�֐���ڑ�*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return SignalConnection{ std::move(connect(func)) };
		}
		/*! �ʏ�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func, boost::placeholders::_1).track_foreign(ins))) };
		}
		/*! �ʏ�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func, boost::placeholders::_1).track_foreign(ins.get_shared()))) };
		}
		/*! �����o�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, RetType(InsType::*m_func)(ArgType)) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins))) };
		}
		/*! �����o�֐���WeakPointer���w�肵�Đڑ�
		
			�C���X�^���X�̌^�́A�����o�֐��̃N���X�^�Ɠ��������̎q�N���X�ł���K�v������B
		*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, RetType(InsType::*m_func)(ArgType)) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins.get_shared()))) };
		}
		/*! �֐��Ăяo��*/
		auto operator()(const ArgType& arg) {
			return signal::operator ()(arg);
		}
	};

	/*! �V�O�i���N���X(void���ꉻ)*/
	template<typename RetType>
	class Signal<RetType(void)> final: private boost::signals2::signal<RetType(void)> {
	public:
		/*! �ʏ�֐���ڑ�*/
		template<typename FuncType>
		SignalConnection ConnectFunction(const FuncType& func) {
			return SignalConnection{ std::move(connect(func)) };
		}
		/*! �ʏ�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const std::shared_ptr<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func).track_foreign(ins))) };
		}
		/*! �ʏ�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType, typename FuncType>
		SignalConnection ConnectFunction(const WeakPointer<InsType>& ins, const FuncType& func) {
			return SignalConnection{ std::move(connect(slot_type(func).track_foreign(ins.get_shared()))) };
		}
		/*! �����o�֐���std::shared_ptr���w�肵�Đڑ�*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const std::shared_ptr<InsType>& ins, RetType(InsType::*m_func)()) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get()).track_foreign(ins))) };
		}
		/*! �����o�֐���WeakPointer���w�肵�Đڑ�*/
		template<class InsType>
		SignalConnection ConnectMemberFunction(const WeakPointer<InsType>& ins, RetType(InsType::*m_func)()) {
			return SignalConnection{ std::move(connect(slot_type(m_func, ins.get()).track_foreign(ins.get_shared()))) };
		}
		/*! �֐��Ăяo��*/
		auto operator()() {
			return signal::operator ()();
		}
	};
}
