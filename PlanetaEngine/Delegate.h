#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"

namespace planeta_engine {
	namespace utility {
		//�C�x���g�n���h�����L�N���X�C���^�[�t�F�C�X(�����N���X)
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
		//WeakPointer�����o�֐��C�x���g�n���h�����L�N���X(�����N���X)
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

		//�����o�֐��C�x���g�n���h�����L�N���X(�����N���X)
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

		//�ʏ�֐��C�x���g�n���h���ۗL�N���X(�����N���X)
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
		//�j���C�x���g�n���h���ۗL�N���X(�����N���X)
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
		//Delegate�ւ̐ڑ��N���X�C���^�[�t�F�X(�����N���X)
		class IDelegateConnecter {
		public:
			IDelegateConnecter()noexcept = default;
			virtual ~IDelegateConnecter()noexcept = default;
			virtual void Remove() = 0;
		};
		//Delegate�ւ̐ڑ��N���X(�����N���X)
		template<typename EventArgType>
		class DelegateConnecter final : public IDelegateConnecter {
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventHandlerHolder<EventArgType>>>;
			DelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list, size_t id)noexcept :handler_list_(handler_list), id_(id) {}
			~DelegateConnecter()noexcept = default;
			/*�o�^�����f���Q�[�g����폜*/
			void Remove()override {
				if (!handler_list_.expired()) {
					auto handler_list_ptr = handler_list_.lock();
					auto it = handler_list_ptr->find(id_);
					assert(it != handler_list_ptr->end()); //������Ȃ��̂͂��������B
					//�Ώ�ID�Ɋ֐��ɕK��false��Ԃ��j���n���h���ێ��N���X��o�^���邱�ƂŁA����C�x���g�������Ƀ��X�g�����菜���Ă��炤�B�������邱�ƂŃC�x���g�������̃n���h���Ăяo�����[�v���ɂ��̊֐����Ăяo����Ă����S�B
					//�����Ńn���h����j�������Ƃ��A�n���h���̏��L�҂�����������������(�����_�����n���h���ɓo�^�����ꍇ�BWeak�|�C���^�ƃ����o�֐��|�C���^�̃Z�b�g��o�^�����ꍇ�͑��ɏ��L�҂�����Ƃ������ƂȂ̂ł����炭���Ă͂܂�Ȃ��B)�n���h���̓������ォ��폜�����B
					//����������Remove()���n���h�����g���Ă�ł����ꍇ�͂���Remove()�𔲂�����Ƀn���h���ɖ߂邽�߁A�������ォ��폜���ꂽ�n���h�������s���Ă��܂����ƂɂȂ蓮�삪���m��ɂȂ�B
					//�V���ɓo�^����j���n���h���ێ��N���X�ɔj���Ώۂ̃n���h���̏��L�����ړ����邱�ƂŁA�����ł̓������ォ��̍폜�͍s�킸���̖����������B
					//�j���Ώۃn���h���͎���C�x���g�������ɔj���n���h���ێ��N���X�ƈꏏ�ɔj������邱�ƂɂȂ�B
					it->second = std::make_unique<GarbageEventHandlerHolder<EventArgType>>(std::move(it->second));
					handler_list_.reset(); //�폜������������A���X�g�ւ̎Q�Ƃ͐؂��Ă���
				}
			}
		private:
			std::weak_ptr<HandlerListType> handler_list_;
			size_t id_;
		};
		/**
		* @brief Delegate�ւ̐ڑ��ێ��N���X(���J�N���X)
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
			* @brief �f���Q�[�g����폜����
			*/
			void Remove() { if (connector_) { connector_->Remove(); connector_.reset(); } }
		private:
			std::shared_ptr<IDelegateConnecter> connector_;
		};
		/**
		* @brief �f���Q�[�g(���J�N���X)
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
			* @brief �f���P�[�g��WeakPointer�����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��WeakPointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			DelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��WeakPointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			DelegateConnection Add(C* c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<MenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�ɒʏ�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
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
			* @brief �f���P�[�g��WeakPointer�����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��WeakPointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			DelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<void, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��Pointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			DelegateConnection Add(C* c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<MenberFunctionEventHandlerHolder<void, C>>(c, f));
				return DelegateConnection(std::make_shared<DelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�ɒʏ�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
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

		/*�f���Q�[�g�n���h���ǉ��N���X(���J�N���X)*/
		template<typename EventArgType = void>
		class DelegateHandlerAdder final{
		public:
			DelegateHandlerAdder(std::function<DelegateConnection(Delegate<EventArgType>&)>&& adder) :delegate_handle_adder_(std::move(adder)) {}
			DelegateConnection operator()(Delegate<EventArgType>& dlgt) { return delegate_handle_adder_(dlgt); }
		private:
			std::function<DelegateConnection(Delegate<EventArgType>&)> delegate_handle_adder_;
		};

		/*WeakMenber�f���Q�[�g�ǉ��N���X�쐬(���J�֐�)*/
		template<typename EventArgType,class C>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(const WeakPointer<C>& c, typename void(C::*f)(EventArgType)) {
			return std::move(DelegateHandlerAdder<EventArgType>([c, f](Delegate<EventArgType>& dlgt) {return dlgt.Add(c, f); }));
		}
		template<class C>
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(const WeakPointer<C>& c, typename void(C::*f)()) {
			return std::move(DelegateHandlerAdder<void>([c, f](Delegate<void>& dlgt) {return dlgt.Add(c, f); }));
		}
		/*MemberFunction�f���Q�[�g�ǉ��N���X�쐬(���J�֐�)*/
		template<typename EventArgType, class C>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(C* c, typename void(C::*f)(EventArgType)) {
			return std::move(DelegateHandlerAdder<EventArgType>([c, f](Delegate<EventArgType>& dlgt) {return dlgt.Add(c, f); }));
		}
		template<class C>
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(C* c, typename void(C::*f)()) {
			return std::move(DelegateHandlerAdder<void>([c, f](Delegate<void>& dlgt) {return dlgt.Add(c, f); }));
		}
		/*�f���Q�[�g�ǉ��N���X�쐬�BEventArg��void�̎��̓e���v���[�g�����Ȃ��A����ȊO�͎w�肵�ė��p����B(���J�֐�)*/
		template<typename EventArgType>
		DelegateHandlerAdder<EventArgType> CreateDelegateHandlerAdder(const std::function<void(EventArgType)>& func) {
			return std::move(DelegateHandlerAdder<EventArgType>([func](Delegate<EventArgType>& dlgt) {return dlgt.Add(func); }));
		}
		inline DelegateHandlerAdder<void> CreateDelegateHandlerAdder(const std::function<void()>& func) {
			return std::move(DelegateHandlerAdder<void>([func](Delegate<void>& dlgt) {return dlgt.Add(func); }));
		}
	}
}
