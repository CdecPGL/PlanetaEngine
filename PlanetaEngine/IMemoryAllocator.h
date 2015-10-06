#pragma once

class IMemoryAllcator{
public:
	IMemoryAllcator() = default;
	virtual ~IMemoryAllcator() = default;
	//�w��T�C�Y�̃��������蓖��(�����Ɍ����w��\�B���s�����ꍇ��_alloc()�ɂ��BadAlloc��O�����o�����)
	template<typename T>
	T* Alloc(size_t num = 1){
		if (num < 1){ return nullptr; }
		void* ptr = _alloc(sizeof(T) * num);
		return reinterpret_cast<T*>(ptr);
	}
	void* Alloc(size_t size){
		if (size == 0){ return nullptr; }
		return _alloc(size);
	}
	void Dealloc(void* ptr){ _dealloc(ptr); } //�w�胁�������(�G���[����BadAlloc��O�𑗏o)
	template<typename T>
	T* New(size_t num = 1){
		T* ptr = Alloc<T>(num);
		for (size_t i = 0; i < num; ++i){
			new(&ptr[i])T();
		}
	}
	template<typename T>
	void Delete(T* ptr){
		ptr->~T();
		Dealloc(ptr);
	}
	virtual bool ReserveMemory(size_t size) = 0; //�������̈�m�ہB�f�[�^�͑S���������
	virtual void ReleaseMemory() = 0; //�m�ۂ����S���������J������
	virtual size_t GetReservedMemorySize()const = 0; //�m�ۂ��ꂽ�������̈�T�C�Y
	virtual size_t GetUsingMemorySize()const = 0;//�g�p����Ă��郁�����T�C�Y(�Ǘ��̈�܂�)
	virtual size_t GetAllocatedMemoryCount()const = 0; //���蓖�Ă�ꂽ�������̐�
	virtual size_t GetAllocatedMemorySize()const = 0; //���蓖�Ă�ꂽ�������T�C�Y���擾(�Ǘ��̈�͊܂܂Ȃ�)
	struct MemoryScanResult{
		bool error = false; //���炩�̃G���[�����邩
		bool corruption = false; //�j������������������������(��������X�L�����𒆎~����)
		bool inconsistency_at_allocated_memory_count = false; //���蓖�Ă����������̕s�����͂��邩
		bool inconsistency_at_allocated_memory_size = false; //���蓖�Ă��������T�C�Y�̕s�����͂��邩
		bool inconsistency_at_using_memory_size = false; //�g�p���������T�C�Y�̕s�����͂��邩
		bool inconsistency_at_reserved_memory_size = false; //�m�ۂ��ꂽ�������T�C�Y�ɑ��������T�C�Y�Ƃ̕s�����͂��邩
		size_t using_memory_size = 0; //�g�p���̃������T�C�Y(�Ǘ��̈�܂�)
		size_t allocated_memory_size = 0; //���蓖�Ă�ꂽ�������T�C�Y(�Ǘ��̈�܂܂Ȃ�)
		size_t free_memory_size = 0; //�g�p���Ă��Ȃ��������T�C�Y(�Ǘ��̈�܂�)
		size_t available_memory_size = 0; //���p�\�ȃ������T�C�Y(�Ǘ��̈�܂܂Ȃ�)
		size_t reserved_memory_size = 0; //�m�ۂ���Ă��鑍�������T�C�Y
		size_t available_max_memory_allocation_size = 0; //���蓖�ĉ\�ȍő�̃������T�C�Y
		double fragment_ratio = 0.0; //�f�Љ���()
	};
	virtual MemoryScanResult Scan()const = 0; //�������̃X�L����
private:
	virtual void* _alloc(size_t) = 0; //���������蓖��
	virtual void _dealloc(void* ptr) = 0; //���������蓖�ĉ���
};