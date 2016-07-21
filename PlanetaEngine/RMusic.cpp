﻿#include "RMusic.h"
#include "DxLib.h"
#include "File.h"

namespace planeta {
	bool RMusic::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		//音楽はすべてメモリ上に保存しておいて、再生時に展開する
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
		}
		_handle = LoadSoundMemByMemImage(file->GetTopPointer(), file->GetSize());
		return _handle >= 0;
	}

	void RMusic::_Dispose() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	int RMusic::GetTotalTimeByMilliSecond() {
		return GetSoundTotalTime(_handle);
	}
}