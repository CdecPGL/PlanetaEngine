#include "planeta/buildin/RSound.hpp"
#include "DxLib.h"
#include "planeta/File.hpp"

namespace plnt {
	bool RSound::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		//効果音はすべてメモリ上に展開
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMNOPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		}
		_handle = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return _handle >= 0;
	}

	void RSound::OnDisposed() {
		if (_handle >= 0) {
			DeleteSoundMem(_handle);
			_handle = -1;
		}
	}

	int RSound::GetTotalTimeByMilliSecond() {
		return GetSoundTotalTime(_handle);
	}
}