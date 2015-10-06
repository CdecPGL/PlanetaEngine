#pragma once

class IMemoryAllcator{
public:
	IMemoryAllcator() = default;
	virtual ~IMemoryAllcator() = default;
	//指定サイズのメモリ割り当て(引数に個数を指定可能。失敗した場合は_alloc()によりBadAlloc例外が送出される)
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
	void Dealloc(void* ptr){ _dealloc(ptr); } //指定メモリ解放(エラー時はBadAlloc例外を送出)
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
	virtual bool ReserveMemory(size_t size) = 0; //メモリ領域確保。データは全消去される
	virtual void ReleaseMemory() = 0; //確保した全メモリを開放する
	virtual size_t GetReservedMemorySize()const = 0; //確保されたメモリ領域サイズ
	virtual size_t GetUsingMemorySize()const = 0;//使用されているメモリサイズ(管理領域含む)
	virtual size_t GetAllocatedMemoryCount()const = 0; //割り当てられたメモリの数
	virtual size_t GetAllocatedMemorySize()const = 0; //割り当てられたメモリサイズを取得(管理領域は含まない)
	struct MemoryScanResult{
		bool error = false; //何らかのエラーがあるか
		bool corruption = false; //破損したメモリが見つかったか(見つけたらスキャンを中止する)
		bool inconsistency_at_allocated_memory_count = false; //割り当てたメモリ数の不整合はあるか
		bool inconsistency_at_allocated_memory_size = false; //割り当てたメモリサイズの不整合はあるか
		bool inconsistency_at_using_memory_size = false; //使用中メモリサイズの不整合はあるか
		bool inconsistency_at_reserved_memory_size = false; //確保されたメモリサイズに総メモリサイズとの不整合はあるか
		size_t using_memory_size = 0; //使用中のメモリサイズ(管理領域含む)
		size_t allocated_memory_size = 0; //割り当てられたメモリサイズ(管理領域含まない)
		size_t free_memory_size = 0; //使用していないメモリサイズ(管理領域含む)
		size_t available_memory_size = 0; //利用可能なメモリサイズ(管理領域含まない)
		size_t reserved_memory_size = 0; //確保されている総メモリサイズ
		size_t available_max_memory_allocation_size = 0; //割り当て可能な最大のメモリサイズ
		double fragment_ratio = 0.0; //断片化率()
	};
	virtual MemoryScanResult Scan()const = 0; //メモリのスキャン
private:
	virtual void* _alloc(size_t) = 0; //メモリ割り当て
	virtual void _dealloc(void* ptr) = 0; //メモリ割り当て解除
};