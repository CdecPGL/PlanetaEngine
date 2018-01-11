#include <cassert>
#include "DxLib.h"
#include "BGMController.hpp"
#include "planeta/buildin/RMusic.hpp"
#include "LogUtility.hpp"

namespace plnt {
	bool BGMController::is_valid() const {
		if (music_resource_ == nullptr) { return false; }
		return music_resource_->is_usable();
	}

	void BGMController::Dispose() {
		if (is_valid()) { StopSoundMem(music_resource_->GetHandle()); }
		music_resource_.reset();
	}

	bool BGMController::is_playing() const {
		if (!is_valid()) { return false; }
		return CheckSoundMem(music_resource_->GetHandle()) == 1 ? true : false;
	}

	bool BGMController::Start(bool loop_flag) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		ChangeVolumeSoundMem(255, music_resource_->GetHandle());
		return Play_(loop_flag);
	}

	bool BGMController::Stop() {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		return StopSoundMem(music_resource_->GetHandle()) >= 0;
	}

	BGMController::~BGMController() {
		Dispose();
	}

	bool BGMController::FadeIn(int frame, bool loop_flag) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		if (is_playing()) { return false; }
		state_ = State_::FadeIn;
		fade_total_frame_ = frame;
		ChangeVolumeSoundMem(0, music_resource_->GetHandle());
		return Play_(loop_flag);
	}

	bool BGMController::FadeOut(int frame) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		if (!is_playing()) { return false; }
		state_ = State_::FadeOut;
		fade_total_frame_ = frame;
		return true;
	}

	void BGMController::Update() {
		switch (state_) {
		case plnt::BGMController::State_::FadeIn:
		{
			double ratio = (double)fade_frame_counter_ / fade_total_frame_;
			ChangeVolumeSoundMem((int)(255 * ratio), music_resource_->GetHandle());
			++fade_frame_counter_;
			break;
		}
		case plnt::BGMController::State_::FadeOut:
		{
			double ratio = 1.0 - (double)fade_frame_counter_ / fade_total_frame_;
			ChangeVolumeSoundMem((int)(255 * ratio), music_resource_->GetHandle());
			++fade_frame_counter_;
			break;
		}
		case plnt::BGMController::State_::None:
			break;
		default:
			assert(false);
			break;
		}
	}

	bool BGMController::Play_(bool loop_flag) {
		return PlaySoundMem(music_resource_->GetHandle(), loop_flag ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) >= 0;
	}

}