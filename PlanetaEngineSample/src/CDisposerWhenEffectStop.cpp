#include "CDisposerWhenEffectStop.hpp"

using namespace plnt;

void CDisposerWhenEffectStop::OnInitialized() {
	Super::OnInitialized();
	auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	auto eff_com = game_object().GetComponent<CEffect>();

	if (eff_com == nullptr) { 
		PE_LOG_ERROR("�G�t�F�N�g�R���|�[�l���g���ݒ肳��Ă��܂���B");
		return;
	}
	task->SetExcuteFunction([this, eff_com] {
		//�쐬���ꂽ����͂܂��Đ����J�n����Ă��Ȃ��\��������̂ŁA��t���[���҂��Ă��画�肷��B
		if (frame_counter_ > 0) {
			if (!eff_com->is_playing()) {
				game_object().Dispose();
			}
		}
		++frame_counter_;
	});
}
