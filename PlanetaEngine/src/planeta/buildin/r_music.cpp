#include "DxLib.h"

#include "planeta/core/file.hpp"

#include "r_music.hpp"

namespace plnt {
	bool r_music::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		//音楽はすべてメモリ上に保存しておいて、再生時に展開する
		if (GetCreateSoundDataType() != DX_SOUNDDATATYPE_MEMPRESS) {
			SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
		}
		handle_ = LoadSoundMemByMemImage(file.top_pointer(), file.size());
		return handle_ >= 0;
	}

	void r_music::on_disposed() {
		if (handle_ >= 0) {
			DeleteGraph(handle_);
			handle_ = -1;
		}
	}

	int r_music::get_total_time_by_milli_second() const { return GetSoundTotalTime(handle_); }
}
