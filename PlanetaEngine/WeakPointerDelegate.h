#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "boost/call_traits.hpp"

#include "WeakPointer.h"

namespace planeta_engine {
	namespace utility {
		//�C�x���g�֐����L�N���X�C���^�[�t�F�C�X
		template<typename EventArgType>
		class IEventFunctionHolder {
		public:
			using ParamType = typename boost::call_traits<EventArgType>::param_type;
			virtual ~IEventFunctionHolder() = default;
			virtual bool Call(ParamType) = 0;
		};

		template<>
		class IEventFunctionHolder<void> {
		public:
			virtual ~IEventFunctionHolder() = default;
			virtual bool Call() = 0;
		};
		//�C�x���g�����o�֐����L�N���X
		template<typename EventArgType,class C>
		class EventMenberFunctionHolder final: public IEventFunctionHolder<EventArgType>{
		public:
			using FunctionType = void(C::*)(typename IEventFunctionHolder<EventArgType>::ParamType);
			EventMenberFunctionHolder(const utility::WeakPointer<C>& wp, FunctionType f):w_ptr_(wp),func_(f) {
			}
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
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
		class EventMenberFunctionHolder<void,C> final : public IEventFunctionHolder<void> {
		public:
			using FunctionType = void(C::*)();
			EventMenberFunctionHolder(const utility::WeakPointer<C>& wp, FunctionType f) :w_ptr_(wp), func_(f) {
			}
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

		//�C�x���g�ʏ�֐��ۗL�N���X
		template<typename EventArgType>
		class EventNormalFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			using FunctionType = std::function<void(typename IEventFunctionHolder<EventArgType>::ParamType)>;
			explicit EventNormalFunctionHolder(const FunctionType& f) :func_(f) {}
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
				func_(e);
				return true;
			}
		private:
			FunctionType func_;
		};
		template<>
		class EventNormalFunctionHolder<void> final : public IEventFunctionHolder<void> {
		public:
			using FunctionType = std::function<void()>;
			explicit EventNormalFunctionHolder(const FunctionType& f) :func_(f) {}
			bool Call()override {
				func_();
				return true;
			}
		private:
			FunctionType func_;
		};
		//�_�~�[�֐��ۗL�N���X
		template<typename EventArgType>
		class EventDumyFunctionHolder final : public IEventFunctionHolder<EventArgType> {
		public:
			bool Call(typename IEventFunctionHolder<EventArgType>::ParamType e)override {
				return false;
			}
		};
		template<>
		class EventDumyFunctionHolder<void> final : public IEventFunctionHolder<void> {
		public:
			bool Call()override {
				return false;
			}
		};
		//WeakPointerDelegate�ւ̐ڑ��N���X�C���^�[�t�F�X
		class IWeakPointerDelegateConnecter {
		public:
			virtual ~IWeakPointerDelegateConnecter() = default;
			virtual void Remove() = 0;
		};
		//WeakPointerDelegate�ւ̐ڑ��N���X
		template<typename EventArgType>
		class WeakPointerDelegateConnecter final : public IWeakPointerDelegateConnecter{
		public:
			using HandlerListType = std::unordered_map<size_t, std::unique_ptr<IEventFunctionHolder<EventArgType>>>;
			WeakPointerDelegateConnecter(const std::weak_ptr<HandlerListType>& handler_list,size_t id) :handler_list_(handler_list),id_(id) {}
			/*�o�^�����f���Q�[�g����폜*/
			void Remove()override {  
				if (!handler_list_.expired()) {
					auto handler_list_ptr = handler_list_.lock();
					auto it = handler_list_ptr->find(id_);
					if (it != handler_list_ptr->end()) {
						//�Ώ�ID�Ɋ֐��ɕK��false��Ԃ��_�~�[��o�^���邱�ƂŁA����C�x���g�������Ƀ��X�g�����菜���Ă��炤�B�������邱�ƂŃC�x���g�������̃n���h���Ăяo�����[�v���ɂ��̊֐����Ăяo����Ă����S�B
						it->second = std::make_unique<EventDumyFunctionHolder<EventArgType>>();
					}
				}
			}
		private:
			std::weak_ptr<HandlerListType> handler_list_;
			size_t id_;
		};
		/**
		* @brief WeakPointerDelegate�ւ̐ڑ��ێ��N���X
		*/
		class WeakPointerDelegateConnection final{
		public:
			explicit WeakPointerDelegateConnection(const std::shared_ptr<IWeakPointerDelegateConnecter>& cp) :connector_(cp) {}
			/**
			* @brief �f���Q�[�g����폜����
			*/
			void Remove() { connector_->Remove(); }
		private:
			std::shared_ptr<IWeakPointerDelegateConnecter> connector_;
		};
		/**
		* @brief WeakPointer�f���Q�[�g
		*/
		template<typename EventArgType = void>
		class WeakPointerDelegate final{
		public:
			WeakPointerDelegate():handlers_(std::make_shared<WeakPointerDelegateConnecter<EventArgType>::HandlerListType>()){}
			void operator()(typename IEventFunctionHolder<EventArgType>::ParamType e) {
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
			WeakPointerDelegateConnection Add(const WeakPointer<C>& c, typename void(C::*f)(typename IEventFunctionHolder<EventArgType>::ParamType)) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventMenberFunctionHolder<EventArgType, C>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			WeakPointerDelegateConnection Add(const std::function<void(typename IEventFunctionHolder<EventArgType>::ParamType)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventNormalFunctionHolder<EventArgType>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<EventArgType>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<EventArgType>::HandlerListType> handlers_;
		};

		template<>
		class WeakPointerDelegate<void> final {
		public:
			WeakPointerDelegate() :handlers_(std::make_shared<WeakPointerDelegateConnecter<void>::HandlerListType>()) {}
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
				handlers_->emplace(id, std::make_unique<EventMenberFunctionHolder<void>>(c, f));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
			/**
			* @brief �f���P�[�g�Ƀ����o�֐���o�^����
			* @param (func) �o�^�������֐�
			* @return �n���h���̍폜�Ɏg���f���Q�[�g�ڑ��N���X
			*/
			WeakPointerDelegateConnection Add(const std::function<void(void)>& func) {
				size_t id = id_counter_++;
				handlers_->emplace(id, std::make_unique<EventNormalFunctionHolder<void>>(func));
				return WeakPointerDelegateConnection(std::make_shared<WeakPointerDelegateConnecter<void>>(handlers_, id));
			}
		private:
			size_t id_counter_ = 0;
			std::shared_ptr<typename WeakPointerDelegateConnecter<void>::HandlerListType> handlers_;
		};
	}
}
