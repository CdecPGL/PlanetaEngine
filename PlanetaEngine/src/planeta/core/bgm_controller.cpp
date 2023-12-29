#include <cassert>

#include "DxLib.h"

#include "bgm_controller.hpp"
#include "planeta/buildin/RMusic.hpp"
#include "log_utility.hpp"

namespace plnt {
	bool bgm_controller::is_valid() const {
		if (music_resource_ == nullptr) { return false; }
		return music_resource_->is_usable();
	}

	void bgm_controller::dispose() {
		if (is_valid()) { StopSoundMem(music_resource_->GetHandle()); }
		music_resource_.reset();
	}

	bool bgm_controller::is_playing() const {
		if (!is_valid()) { return false; }
		return CheckSoundMem(music_resource_->GetHandle()) == 1 ? true : false;
	}

	bool bgm_controller::start(const bool loop_flag) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		ChangeVolumeSoundMem(255, music_resource_->GetHandle());
		return play(loop_flag);
	}

	bool bgm_controller::stop() {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		return StopSoundMem(music_resource_->GetHandle()) >= 0;
	}

	bgm_controller::~bgm_controller() { dispose(); }

	bool bgm_controller::fade_in(const int frame, const bool loop_flag) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		if (is_playing()) { return false; }
		state_ = state::fade_in;
		fade_total_frame_ = frame;
		ChangeVolumeSoundMem(0, music_resource_->GetHandle());
		return play(loop_flag);
	}

	bool bgm_controller::fade_out(const int frame) {
		if (!is_valid()) {
			PE_LOG_WARNING("コントローラは無効です。");
			return false;
		}
		if (!is_playing()) { return false; }
		state_ = state::fade_out;
		fade_total_frame_ = frame;
		return true;
	}

	void bgm_controller::update() {
		switch (state_) {
			case state::fade_in: {
				const double ratio = static_cast<double>(fade_frame_counter_) / fade_total_frame_;
				ChangeVolumeSoundMem(static_cast<int>(255 * ratio), music_resource_->GetHandle());
				++fade_frame_counter_;
				break;
			}
			case state::fade_out: {
				const double ratio = 1.0 - static_cast<double>(fade_frame_counter_) / fade_total_frame_;
				ChangeVolumeSoundMem(static_cast<int>(255 * ratio), music_resource_->GetHandle());
				++fade_frame_counter_;
				break;
			}
			case state::none:
				break;
		}
	}

	bool bgm_controller::play(const bool loop_flag) const {
		return PlaySoundMem(music_resource_->GetHandle(), loop_flag ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK) >= 0;
	}
}
