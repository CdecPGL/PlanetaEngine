#pragma once

#include <memory>
#include <typeinfo>
#include <cassert>
#include <type_traits>
#include "NullWeakPointerException.h"


namespace planeta_engine {
	namespace utility {
		template<typename T>
		class WeakPointer;

		template<typename T0,typename T1>
		WeakPointer<T0> static_weak_pointer_cast(const WeakPointer<T1>& ptr);
		template<typename T0,typename T1>
		WeakPointer<T0> dynamic_weak_pointer_cast(const WeakPointer<T1>& ptr);

		template<typename T>
		class WeakPointer {
			template<typename T0,typename T1> friend  WeakPointer<T0> static_weak_pointer_cast(const WeakPointer<T1>& ptr);
			template<typename T0,typename T1> friend  WeakPointer<T0> dynamic_weak_pointer_cast(const WeakPointer<T1>& ptr);
			template<typename T0> friend class WeakPointer;
		public:
			WeakPointer() = default;
			WeakPointer(std::nullptr_t) {}
			template<typename T1>
			WeakPointer(const WeakPointer<T1>& wp) :w_ptr_(wp.w_ptr_) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			template<typename T1>
			WeakPointer(const std::weak_ptr<T1>& wp) :w_ptr_(wp) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			template<typename T1>
			WeakPointer(const std::shared_ptr<T1>& sp) :w_ptr_(sp) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			/*�X�}�|���擾*/
			std::shared_ptr<T>get_shared()const { return w_ptr_.lock(); }
			std::weak_ptr<T>get_weak()const { return w_ptr_; }
			/*�|�C���^�[���擾*/
			T* get()const { return w_ptr_._Get(); }
			//////////�A�N�Z�X���Z�q//////////
			T* operator->() {
				std::shared_ptr<T> s_ptr = w_ptr_.lock();
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(s_ptr != nullptr);
#else //����ȊO�ł͗�O�𓊂���
				if (s_ptr == nullptr) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif
				return s_ptr.get();
			}
			const T* operator->()const {
				std::shared_ptr<T> s_ptr = w_ptr_.lock();
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(s_ptr != nullptr);
#else
				if (s_ptr == nullptr) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return s_ptr.get();
			}
			//////////�|�C���^�Q�Ɖ��Z�q//////////
			T& operator*()const {
				std::shared_ptr<T> s_ptr = w_ptr_.lock();
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(s_ptr != nullptr);
#else
				if (s_ptr == nullptr) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return *s_ptr;
			}
			operator bool()const {
				return !w_ptr_.expired();
			}
			bool operator==(bool b)const {
				return operator bool() == b;
			}
			bool operator!=(bool b)const {
				return !((*this) == b);
			}
			bool operator==(std::nullptr_t)const {
				return w_ptr_._Get() == nullptr;
			}
			bool operator!=(std::nullptr_t)const {
				return !((*this) == nullptr);
			}

			using pointer_type = T;
		private:
			std::weak_ptr<T> w_ptr_;
		};

		template<typename T0,typename T1>
		WeakPointer<T0> static_weak_pointer_cast(const WeakPointer<T1>& ptr) {
			return WeakPointer<T0>(std::static_pointer_cast<T0>(ptr.w_ptr_.lock()));
		}
		template<typename T0, typename T1>
		WeakPointer<T0> dynamic_weak_pointer_cast(const WeakPointer<T1>& ptr) {
			return WeakPointer<T0>(std::dynamic_pointer_cast<T0>(ptr.w_ptr_.lock()));
		}

		template<typename T>
		WeakPointer<T> MakeWeak(const std::shared_ptr<T>& s_ptr) {
			return WeakPointer<T>(s_ptr);
		}
	}
}
