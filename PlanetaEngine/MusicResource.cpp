#include "MusicResource.h"
#include "DxLib.h"
#include "File.h"

namespace planeta_engine {
	namespace resources {

		bool MusicResource::_Create(const std::shared_ptr<const file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			//‰¹Šy‚Í‚·‚×‚Äƒƒ‚ƒŠã‚É•Û‘¶‚µ‚Ä‚¨‚¢‚ÄAÄ¶Žž‚É“WŠJ‚·‚é
			if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMPRESS) {
				SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
			}
			_handle = LoadSoundMemByMemImage(file->GetTopPointer(), file->GetSize());
			return _handle >= 0;
		}

		void MusicResource::_Dispose()
		{
			if (_handle >= 0) {
				DeleteGraph(_handle);
				_handle = -1;
			}
		}

		int MusicResource::GetTotalTimeByMilliSecond()
		{
			return GetSoundTotalTime(_handle);
		}

	}
}
