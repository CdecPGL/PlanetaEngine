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
			/*���|�͎󂯕t���邪�A�K�������ȃ|�C���^�ɂȂ�B(nullptr�p)*/
			WeakPointer(const void* p){}
			template<typename T1>
			WeakPointer(const WeakPointer<T1>& wp) :_w_ptr(wp._w_ptr) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			template<typename T1>
			WeakPointer(const std::weak_ptr<T1>& wp) :_w_ptr(wp) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			template<typename T1>
			WeakPointer(const std::shared_ptr<T1>& sp) :_w_ptr(sp) { static_assert(std::is_base_of<T, T1>::value == true, "T is not base of T1."); }
			/*�X�}�|���擾*/
			std::shared_ptr<T>get_shared()const { return _w_ptr.lock(); }
			std::weak_ptr<T>get_weak()const { return _w_ptr; }
			/*�|�C���^�[���擾*/
			T* get()const { return _w_ptr._Get(); }
			//////////�A�N�Z�X���Z�q//////////
			T* operator->() {
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(_w_ptr.expired() == false);
#else
				if (_w_ptr.expired()) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return _w_ptr.lock().get();
			}
			const T* operator->()const {
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(_w_ptr.expired() == false);
#else
				if (_w_ptr.expired()) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return _w_ptr.lock().get();
			}
			//////////�|�C���^�Q�Ɖ��Z�q//////////
			T& operator*() {
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(_w_ptr.expired() == false);
#else
				if (_w_ptr.expired()) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return *_w_ptr.lock();
			}
			const T& operator*()const {
#ifdef _DEBUG //NULL�Q�Ƃ����������ꍇ�ADebug�r���h����assert���A
				assert(_w_ptr.expired() == false);
#else
				if (_w_ptr.expired()) {
					throw NullWeakPointerException(std::string("Null weak pointer is called : ") + typeid(T).name());
				}
#endif //����ȊO�ł͗�O�𓊂���
				return *_w_ptr.lock();
			}
			operator bool()const {
				return !_w_ptr.expired();
			}
			operator void*()const {
				return _w_ptr._Get();
			}

			using pointer_type = T;
		private:
			std::weak_ptr<T> _w_ptr;
		};

		template<typename T0,typename T1>
		WeakPointer<T0> static_weak_pointer_cast(const WeakPointer<T1>& ptr) {
			return WeakPointer<T0>(std::static_pointer_cast<T0>(ptr._w_ptr.lock()));
		}
		template<typename T0, typename T1>
		WeakPointer<T0> dynamic_weak_pointer_cast(const WeakPointer<T1>& ptr) {
			return WeakPointer<T0>(std::dynamic_pointer_cast<T0>(ptr._w_ptr.lock()));
		}
	}
}
