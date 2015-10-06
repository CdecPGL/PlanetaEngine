#pragma once
#include <string>
#include <typeinfo>

#undef max //windows.h�Œ�`����Ă���max�}�N���𖳌���(std::numeric_limits<size_t>::max()�̂���)

namespace planeta_engine{
	namespace core{
		class Object
		{
		public:
			Object() = default;
			virtual ~Object() = default;
			/*�C���X�^���X���������m�F*/
			bool Equals(const Object* o)const{ return o == this; }
			/*�C���X�^���X���������m�F*/
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

				//�R���X�g���N�^
				allocator() throw(){}
				allocator(const allocator<T>&) throw(){}
				template <class U> allocator(const allocator<U>&) throw(){}
				// �f�X�g���N�^
				~allocator() throw(){}

				/* �A���P�[�^��U�^�Ƀo�C���h���� */
				template <class U>
				struct rebind
				{
					typedef allocator<U> other;
				};

				/* �����������蓖�Ă� */
				pointer allocate(size_type num, const void* hint = 0)
				{
					return (pointer)(T::operator new(num * sizeof(T)));
				}

				/* ���蓖�čς݂̗̈������������ */
				void construct(pointer p, const T& value)
				{
					new ((void*)p) T(value);
				}

				/* �I�u�W�F�N�g�̃A�h���X��Ԃ� */
				pointer address(reference value) const
				{
					return &value;
				}

				/* const�ȃI�u�W�F�N�g�̃A�h���X��Ԃ� */
				const_pointer address(const_reference value) const
				{
					return &value;
				}

				/* �������ς݂̗̈���폜���� */
				void destroy(pointer p)
				{
					p->~T();
				}

				/* ��������������� */
				void deallocate(pointer p, size_type n)
				{
					T::operator delete((void*)p);
				}

				/* ���蓖�Ă邱�Ƃ��ł���ő�̃T�C�Y��Ԃ� */
				size_type max_size() const throw()
				{
					/* numeric_limits��size_t�̂Ƃ肤��ő�l���擾 */
					return std::numeric_limits<size_t>::max() / sizeof(T);
				}
			};

		private:
			static size_t _reserved_memory_byte; //�m�ۂ��ꂽ�������̃T�C�Y
			static void* _allocate(size_t s);
			static void _deallocate(void* p);
		};
	}
}

