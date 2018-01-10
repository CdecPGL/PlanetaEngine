#include "RMusic.hpp"
#include "DxLib.h"
#include "File.hpp"

namespace plnt {
	bool RMusic::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		//音楽はすべてメモリ上に保存しておいて、再生時に展開する
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
		}
		_handle = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return _handle >= 0;
	}

	void RMusic::OnDisposed() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	int RMusic::GetTotalTimeByMilliSecond() {
		return GetSoundTotalTime(_handle);
	}
}