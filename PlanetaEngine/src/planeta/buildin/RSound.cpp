#include "DxLib.h"

#include "planeta/core/file.hpp"

#include "RSound.hpp"

namespace plnt {
	bool RSound::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		//効果音はすべてメモリ上に展開
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMNOPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		}
		_handle = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return _handle >= 0;
	}

	void RSound::on_disposed() {
		if (_handle >= 0) {
			DeleteSoundMem(_handle);
			_handle = -1;
		}
	}

	int RSound::GetTotalTimeByMilliSecond() { return GetSoundTotalTime(_handle); }
}
