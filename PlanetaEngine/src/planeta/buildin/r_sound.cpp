#include "DxLib.h"

#include "planeta/core/file.hpp"

#include "r_sound.hpp"

namespace plnt {
	bool r_sound::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		//効果音はすべてメモリ上に展開
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMNOPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
		}
		handle_ = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return handle_ >= 0;
	}

	void r_sound::on_disposed() {
		if (handle_ >= 0) {
			DeleteSoundMem(handle_);
			handle_ = -1;
		}
	}

	int r_sound::get_total_time_by_milli_second() const { return GetSoundTotalTime(handle_); }
}
