#include "DxLib.h"

#include "sound_effect_controller.hpp"
#include "planeta/buildin/RSound.hpp"
#include "log_utility.hpp"

namespace plnt {
	void sound_effect_controller::dispose() {
		for (const auto &dh : duplicated_dx_handles_) { DeleteSoundMem(dh); }
		sound_resource_.reset();
	}

	bool sound_effect_controller::play() {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		//空いているサウンドを先頭から探していく
		int64_t usable_index = -1;
		for (size_t i = 0; i < duplicated_dx_handles_.size(); ++i) {
			if (CheckSoundMem(duplicated_dx_handles_[i]) == 0) {
				usable_index = static_cast<int64_t>(i);
				break;
			}
		}
		if (usable_index < 0) {
			//空いているサウンドがなかったら新たに作成
			const int ndh = DuplicateSoundMem(sound_resource_->GetHandle());
			duplicated_dx_handles_.push_back(ndh);
			usable_index = static_cast<int64_t>(duplicated_dx_handles_.size()) - 1;
		}
		//ボリュームを算出(最初二つの音はフルボリューム、3つめ以降は半減していく)
		const double volume = usable_index <= 1 ? 1.0 : 1.0 / pow(2, (usable_index - 1));
		ChangeNextPlayVolumeSoundMem(static_cast<int>(255 * volume), duplicated_dx_handles_[usable_index]);
		PlaySoundMem(duplicated_dx_handles_[usable_index], DX_PLAYTYPE_BACK);
		return true;
	}

	sound_effect_controller::~sound_effect_controller() { dispose(); }

	bool sound_effect_controller::is_valid() const {
		if (sound_resource_ == nullptr) { return false; }
		return sound_resource_->is_usable();
	}
}
