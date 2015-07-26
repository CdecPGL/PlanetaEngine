#include "Object.h"

namespace PlanetaEngine{
	namespace Core{
		size_t Object::_reserved_memory_byte = 0;
		void* Object::_allocate(size_t s){
			size_t header_size = sizeof(size_t); //ヘッダサイズ確認
			size_t total_size = header_size + s; //確保する合計サイズ
			void* ptr = malloc(total_size); //データサイズ＋ヘッダサイズのメモリを確保
			_reserved_memory_byte += total_size; //確保済サイズを加算
			(*reinterpret_cast<size_t*>(ptr)) = total_size; //ヘッダに確保したサイズを記録
			return reinterpret_cast<char*>(ptr)+header_size; //データ本体のポインタを返す
		}
		void Object::_deallocate(void* p){
			size_t header_size = sizeof(size_t); //ヘッダサイズ確認
			p = reinterpret_cast<char*>(p)-header_size; //ヘッダ先頭へ移動
			size_t s = (*reinterpret_cast<size_t*>(p)); //サイズ情報取得
			_reserved_memory_byte -= s; //確保済サイズを減算
			return free(p);
		}
	}
}