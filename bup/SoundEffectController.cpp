#include "SoundEffectController.h"
#include "RSound.h"
#include "DxLib.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {
		void SoundEffectController::Dispose()
		{
			for (auto& dh : duplicated_dx_handles_) {
				DeleteSoundMem(dh);
			}
			sound_resource_.reset();
		}

		bool SoundEffectController::Play()
		{
			if (!is_valid()) {
				PE_LOG_WARNING("コントローラは無効です。");
				return false;
			}
			//空いているサウンドを先頭から探していく
			int usable_index = -1;
			for (size_t i = 0; i < duplicated_dx_handles_.size();++i) {
				if (CheckSoundMem(duplicated_dx_handles_[i]) == 0) {
					usable_index = i;
					break;
				}
			}
			if (usable_index < 0) { //空いているサウンドがなかったら新たに作成
				int ndh = DuplicateSoundMem(sound_resource_->GetHandle());
				duplicated_dx_handles_.push_back(ndh);
				usable_index = duplicated_dx_handles_.size() - 1;
			}
			//ボリュームを算出(最初二つの音はフルボリューム、3つめ以降は半減していく)
			double volume = usable_index <= 1 ? 1.0 : 1.0 / pow(2, (usable_index - 1));
			ChangeNextPlayVolumeSoundMem((int)(255 * volume), duplicated_dx_handles_[usable_index]);
			PlaySoundMem(duplicated_dx_handles_[usable_index], DX_PLAYTYPE_BACK);			
			return true;
		}

		SoundEffectController::~SoundEffectController()
		{
			Dispose();
		}

		bool SoundEffectController::is_valid() const
		{
			if (sound_resource_ == nullptr) { return false; }
			return sound_resource_->is_usable();
		}

	}
}