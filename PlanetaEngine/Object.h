#pragma once
#include <string>
#include <typeinfo>
#include <memory>
#include "Reflection.h"

#undef max //windows.hで定義されているmaxマクロを無効化(std::numeric_limits<size_t>::max()のため)

namespace planeta {
	namespace core {
		class IMemoryAllcator;
		class Object {
		public:
			Object() = default;
			virtual ~Object() = default;
			/*インスタンスが同じか確認*/
			bool Equals(const Object* o)const { return o == this; }
			/*インスタンスが同じか確認*/
			bool Equals(const Object& o)const { return &o == this; }
			virtual std::string ToString()const { return typeid(*this).name(); }
			const std::type_info& GetType()const { return typeid(*this); }
			size_t GetHashCode()const { return typeid(*this).hash_code(); }

			/*static void* operator new (size_t s){ return _allocate(s); }
			static void operator delete (void* p){ _deallocate(p); }
			static void* operator new[](size_t s){return _allocate(s); }
			static void operator delete[](void* p){_deallocate(p); }*/

			//			static const IMemoryAllcator& memory_allocator();

			template <class T>
			struct Allocator {
				// 要素の型
				using value_type = T;

				// 特殊関数
				// (デフォルトコンストラクタ、コピーコンストラクタ
				//  、ムーブコンストラクタ)
				Allocator() {}

				// 別な要素型のアロケータを受け取るコンストラクタ
				template <class U>
				Allocator(const Allocator<U>&) {}

				// メモリ確保
				T* allocate(std::size_t n) {
					return reinterpret_cast<T*>(_allocate(sizeof(T)*n));
				}

				// メモリ解放
				void deallocate(T* p, std::size_t n) {
					static_cast<void>(n);
					_deallocate(p);
				}
			};

			/*template<typename C, typename... Args>
			std::shared_ptr<C> make_shared_object(Args&&... args) { return std::allocate_shared<C,Allocator<C>,Args...>(Allocator<C>(), std::forward<Args>(args)...); };
			template<typename C>
			std::shared_ptr<C> make_shared_object() { return std::allocate_shared<C, Allocator<C>>(Allocator<C>()); };*/


		private:
			static void* _allocate(size_t s);
			static void _deallocate(void* p);
		};

		// 比較演算子
		template <class T, class U>
		bool operator==(const Object::Allocator<T>&, const Object::Allocator<U>&) {
			return true;
		}

		template <class T, class U>
		bool operator!=(const Object::Allocator<T>&, const Object::Allocator<U>&) {
			return false;
		}
	}
}

