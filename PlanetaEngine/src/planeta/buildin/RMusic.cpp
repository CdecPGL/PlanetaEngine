#include "DxLib.h"

#include "planeta/core/file.hpp"

#include "RMusic.hpp"

namespace plnt {
	bool RMusic::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		//音楽はすべてメモリ上に保存しておいて、再生時に展開する
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
		}
		_handle = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return _handle >= 0;
	}

	void RMusic::on_disposed() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	int RMusic::GetTotalTimeByMilliSecond() { return GetSoundTotalTime(_handle); }
}
