#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"
#include "PEAssert.h"

namespace planeta_engine {
	namespace utility {
		//�C�x���g�n���h�����L�N���X�C���^�[�t�F�C�X
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
		//WeakPointer�����o�֐��C�x���g�n���h�����L�N���X
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

		//�ʏ�֐��C�x���g�n���h���ۗL�N���X
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
		//�j���C�x���g�n���h���ۗL�N���X
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
		//WeakPointerDelegate�ւ̐ڑ��N���X�C���^�[�t�F�X
		class IWeakPointerDelegateConnecter {
		public:
			IWeakPointerDelegateConnecter()noexcept = default;
			virtual ~IWeakPointerDelegateConnecter()noexcept = default;
			virtual void Remove() = 0;
		};
		//WeakPointerDelegate�ւ̐ڑ��N���X
		template<typename EventArgType>
		class WeakPointerDelegateConnecter final : public IWeakPointerDelegateConnecter {
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventHandlerHolder<EventArgType>>>;
			WeakPointerDelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list, size_t id)noexcept :handler_list_(handler_list), id_(id) {}
			~WeakPointerDelegateConnecter()noexcept = default;
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
		* @brief WeakPointerDelegate�ւ̐ڑ��ێ��N���X
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
			* @brief �f���Q�[�g����폜����
			*/
			void Remove() { if (connector_) { connector_->Remove(); connector_.reset(); } }
		private:
			std::shared_ptr<IWeakPointerDelegateConnecter> connector_;
		};
		/**
		* @brief WeakPointer�f���Q�[�g
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
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��WeakPointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(HandlerParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<EventArgType, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
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
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (c) �I�u�W�F�N�g��WeakPointer
			* @param (f) �����o�֐��|�C���^
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			template<class C>
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)()) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<WeakPointerMenberFunctionEventHandlerHolder<void, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
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
