#include "RSound.h"
#include "DxLib.h"
#include "File.h"

namespace planeta {
	bool RSound::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		//効果音はすべてメモリ上に展開
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMNOPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		}
		_handle = LoadSoundMemByMemImage(file->GetTopPointer(), file->GetSize());
		return _handle >= 0;
	}

	void RSound::_Dispose() {
		if (_handle >= 0) {
			DeleteSoundMem(_handle);
			_handle = -1;
		}
	}

	int RSound::GetTotalTimeByMilliSecond() {
		return GetSoundTotalTime(_handle);
	}
}