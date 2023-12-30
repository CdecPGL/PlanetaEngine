#pragma once

#include "boost/signals2/signal.hpp"

#include "WeakPointer.hpp"

namespace plnt {
	/*! シグナルへの接続クラス*/
	class signal_connection final {
	public:
		/*! デフォルトコンストラクタ*/
		signal_connection() = default;
		/*! コピーコンストラクタ*/
		signal_connection(const signal_connection &obj) = default;

		/*! ムーブコンストラクタ*/
		signal_connection(signal_connection &&obj) noexcept : boost_sig2_connection_(
			std::move(obj.boost_sig2_connection_)) { }

		/*! コンストラクタ*/
		explicit signal_connection(boost::signals2::connection &&bsc) : boost_sig2_connection_(std::move(bsc)) { }

		/*! デストラクタ*/
		~signal_connection() = default;

		/*! 代入演算子*/
		signal_connection &operator=(const signal_connection &obj) = default;

		/*! ムーブ演算子*/
		signal_connection &operator=(signal_connection &&obj) noexcept {
			boost_sig2_connection_ = std::move(obj.boost_sig2_connection_);
			return *this;
		}

		/*! シグナルへの接続を切断する*/
		void disconnect() const { boost_sig2_connection_.disconnect(); }

		/*! シグナルへ接続されているかを取得*/
		[[nodiscard]] bool is_connected() const { return boost_sig2_connection_.connected(); }

	private:
		boost::signals2::connection boost_sig2_connection_;
	};

	/*! シグナルクラス*/
	template <typename SigType>
	class signal final : boost::signals2::signal<SigType> {
	public:
		//シグナル関数型の引数の個数が1以下か確認
		static_assert(boost::function_traits<SigType>::arity <= 1,
		              "The arity of signal function must be less than or equal to 1");
		/*! シグナル関数の引数型*/
		using arg_type = typename boost::function_traits<SigType>::arg1_type;
		/*! シグナル関数の戻り値型*/
		using ret_type = typename boost::function_traits<SigType>::result_type;

		using my_signal = boost::signals2::signal<SigType>;
		using slot_type = typename my_signal::slot_type;
		/*! 通常関数を接続*/
		template <typename FuncType>
		signal_connection connect_function(const FuncType &func) {
			return signal_connection{std::move(my_signal::connect(func))};
		}

		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template <class InsType, typename FuncType>
		signal_connection connect_function(const std::shared_ptr<InsType> &ins, const FuncType &func) {
			return signal_connection{
				std::move(my_signal::connect(
					slot_type(std::function<SigType>(func), boost::placeholders::_1).track_foreign(ins)))
			};
		}

		/*! 通常関数をWeakPointerを指定して接続*/
		template <class InsType, typename FuncType>
		signal_connection connect_function(const WeakPointer<InsType> &ins, const FuncType &func) {
			return signal_connection{
				std::move(my_signal::connect(
					slot_type(std::function<SigType>(func), boost::placeholders::_1).track_foreign(ins.get_shared())))
			};
		}

		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template <class InsType>
		signal_connection
		connect_member_function(const std::shared_ptr<InsType> &ins, ret_type (InsType::*m_func)(arg_type)) {
			return signal_connection{
				std::move(my_signal::connect(slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins)))
			};
		}

		/*! メンバ関数をWeakPointerを指定して接続
		
			インスタンスの型は、メンバ関数のクラス型と同じかその子クラスである必要がある。
		*/
		template <class InsType>
		signal_connection connect_member_function(const WeakPointer<InsType> &ins,
		                                          ret_type (InsType::*m_func)(arg_type)) {
			return signal_connection{
				std::move(my_signal::connect(
					slot_type(m_func, ins.get(), boost::placeholders::_1).track_foreign(ins.get_shared())))
			};
		}

		/*! 関数呼び出し*/
		auto operator()(const arg_type &arg) { return my_signal::operator ()(arg); }
	};

	/*! シグナルクラス(void特殊化)*/
	template <typename RetType>
	class signal<RetType()> final : boost::signals2::signal<RetType()> {
	public:
		using my_signal = boost::signals2::signal<RetType()>;
		using slot_type = typename my_signal::slot_type;

		/*! 通常関数を接続*/
		template <typename FuncType>
		signal_connection connect_function(const FuncType &func) {
			return signal_connection{std::move(my_signal::connect(func))};
		}

		/*! 通常関数をstd::shared_ptrを指定して接続*/
		template <class InsType, typename FuncType>
		signal_connection connect_function(const std::shared_ptr<InsType> &ins, const FuncType &func) {
			return signal_connection{
				std::move(my_signal::connect(slot_type(std::function<RetType()>(func)).track_foreign(ins)))
			};
		}

		/*! 通常関数をWeakPointerを指定して接続*/
		template <class InsType, typename FuncType>
		signal_connection connect_function(const WeakPointer<InsType> &ins, const FuncType &func) {
			return signal_connection{
				std::move(my_signal::connect(
					slot_type(std::function<RetType()>(func)).track_foreign(ins.get_shared())))
			};
		}

		/*! メンバ関数をstd::shared_ptrを指定して接続*/
		template <class InsType>
		signal_connection connect_member_function(const std::shared_ptr<InsType> &ins, RetType (InsType::*m_func)()) {
			return signal_connection{std::move(my_signal::connect(slot_type(m_func, ins.get()).track_foreign(ins)))};
		}

		/*! メンバ関数をWeakPointerを指定して接続*/
		template <class InsType>
		signal_connection connect_member_function(const WeakPointer<InsType> &ins, RetType (InsType::*m_func)()) {
			return signal_connection{
				std::move(my_signal::connect(slot_type(m_func, ins.get()).track_foreign(ins.get_shared())))
			};
		}

		/*! 関数呼び出し*/
		auto operator()() { return my_signal::operator ()(); }
	};
}
