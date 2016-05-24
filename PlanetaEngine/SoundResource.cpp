#include "SoundResource.h"
#include "DxLib.h"
#include "File.h"

namespace planeta_engine {
	namespace resources {

		bool SoundResource::_Create(const std::shared_ptr<const File>& file)
		{
			if (file->GetStatus() != File::FileStatus::Available) { return false; }
			//効果音はすべてメモリ上に展開
			if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMNOPRESS) {
				SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
			}
			_handle = LoadSoundMemByMemImage(file->GetTopPointer(), file->GetSize());
			return _handle >= 0;
		}

		void SoundResource::_Dispose()
		{
			if (_handle >= 0) {
				DeleteSoundMem(_handle);
				_handle = -1;
			}
		}

		int SoundResource::GetTotalTimeByMilliSecond()
		{
			return GetSoundTotalTime(_handle);
		}

	}
}