#pragma once
#include <string>
#include <typeinfo>

#undef max //windows.hで定義されているmaxマクロを無効化(std::numeric_limits<size_t>::max()のため)

namespace planeta_engine{
	namespace core{
		class Object
		{
		public:
			Object() = default;
			virtual ~Object() = default;
			/*インスタンスが同じか確認*/
			bool Equals(const Object* o)const{ return o == this; }
			/*インスタンスが同じか確認*/
			bool Equals(const Object& o)const{ return &o == this; }
			virtual std::string ToString()const{ return typeid(*this).name(); }
			const std::type_info& GetType()const{ return typeid(*this); }
			size_t GetHashCode()const{ return typeid(*this).hash_code(); }

//			static void* operator new (size_t s){ return _allocate(s); }
//			static void operator delete (void* p){ _deallocate(p); }
//			static void* operator new[](size_t s){return _allocate(s); }
//			static void operator delete[](void* p){_deallocate(p); }

			size_t static GetTotalAllocatedMemoryByte(){ return _reserved_memory_byte; }
			template <class T> class allocator
			{
			public:
				typedef size_t  size_type;
				typedef ptrdiff_t difference_type;
				typedef T* pointer;
				typedef const T* const_pointer;
				typedef T& reference;
				typedef const T& const_reference;
				typedef T value_type;

				//コンストラクタ
				allocator() throw(){}
				allocator(const allocator<T>&) throw(){}
				template <class U> allocator(const allocator<U>&) throw(){}
				// デストラクタ
				~allocator() throw(){}

				/* アロケータをU型にバインドする */
				template <class U>
				struct rebind
				{
					typedef allocator<U> other;
				};

				/* メモリを割り当てる */
				pointer allocate(size_type num, const void* hint = 0)
				{
					return (pointer)(T::operator new(num * sizeof(T)));
				}

				/* 割り当て済みの領域を初期化する */
				void construct(pointer p, const T& value)
				{
					new ((void*)p) T(value);
				}

				/* オブジェクトのアドレスを返す */
				pointer address(reference value) const
				{
					return &value;
				}

				/* constなオブジェクトのアドレスを返す */
				const_pointer address(const_reference value) const
				{
					return &value;
				}

				/* 初期化済みの領域を削除する */
				void destroy(pointer p)
				{
					p->~T();
				}

				/* メモリを解放する */
				void deallocate(pointer p, size_type n)
				{
					T::operator delete((void*)p);
				}

				/* 割り当てることができる最大のサイズを返す */
				size_type max_size() const throw()
				{
					/* numeric_limitsでsize_tのとりうる最大値を取得 */
					return std::numeric_limits<size_t>::max() / sizeof(T);
				}
			};

		private:
			static size_t _reserved_memory_byte; //確保されたメモリのサイズ
			static void* _allocate(size_t s);
			static void _deallocate(void* p);
		};
	}
}

