#pragma once
#include "IMemoryAllocator.hpp"
#include <cmath>
#include <vector>
#include<new>
#include "BadAllocException.hpp"

//http://marupeke296.com/ALG_No2_TLSFMemoryAllocator.html
//ポインタが32bitであることが前提となっている。x64プログラムにするには修正が必要

namespace planeta {
	namespace private_ {
		class TLSFMemoryAllocator : public IMemoryAllcator {
		public:
			TLSFMemoryAllocator();
			TLSFMemoryAllocator(size_t size);
			~TLSFMemoryAllocator();
			bool ReserveMemory(size_t size)override; //メモリ領域確保。データは全消去される
			void ReleaseMemory()override; //確保した全メモリを開放する
			size_t GetReservedMemorySize()const override { return  _memory_pool_size; } //確保されたメモリ領域サイズ
			size_t GetUsingMemorySize()const override { return _using_memory_size; } //使用されているメモリサイズ(管理領域含む)
			size_t GetAllocatedMemoryCount()const override { return _allocated_memory_count; } //割り当てられたメモリの数
			size_t GetAllocatedMemorySize()const override { return _allcated_memory_size; } //割り当てられたメモリサイズを取得(管理領域は含まない)
			MemoryScanResult Scan()const override { return DetailScan(); }
			struct MemoryDetailScanResult : public MemoryScanResult {
				bool inconsistency_at_dumy_count = false; //ダミー数の不整合はあるか
				bool inconsistency_at_free_block_list = false; //フリーブロックリストに登録されているブロック数の不整合はあるか
				bool inconsistency_at_first_free_block_list_bit_array = false; //第一フリーブロックリストビットの不整合が存在するか
				bool inconsistency_at_second_free_block_list_bit_array = false; //第二フリーブロックリストビットの不整合が存在するか
				size_t total_block_count = 0; //総ブロック数(ダミー二つも含む)
				size_t free_block_count = 0; //フリーブロック数
				size_t active_block_count = 0; //使用中ブロック数(ダミー二つは含まない)
				size_t dumy_block_count = 0; //ダミーブロック(サイズが0)数
			};
			MemoryDetailScanResult DetailScan()const; //メモリの詳細スキャン

		private:
			class _Impl;
			_Impl* _impl;

			char* _memory_pool; //確保した領域の先頭ポインタ
			size_t _memory_pool_size; //確保した領域のサイズ

			size_t _using_memory_size; //使用中のメモリサイズ(管理領域を含む)
			size_t _allcated_memory_size; //割り当てられたメモリサイズ(管理領域を含まない)
			size_t _allocated_memory_count; //割り当てたメモリの総数

			void* _alloc(size_t)override; //メモリ割り当て
			void _dealloc(void* ptr)override; //メモリ割り当て解除
		};
	}
}