#include "CDisposerWhenEffectStop.hpp"

using namespace plnt;

void CDisposerWhenEffectStop::OnInitialized() {
	Super::OnInitialized();
	auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	auto eff_com = game_object().GetComponent<CEffect>();

	if (eff_com == nullptr) { 
		PE_LOG_ERROR("エフェクトコンポーネントが設定されていません。");
		return;
	}
	task->SetExcuteFunction([this, eff_com] {
		//作成された直後はまだ再生が開始されていない可能性があるので、一フレーム待ってから判定する。
		if (frame_counter_ > 0) {
			if (!eff_com->is_playing()) {
				game_object().Dispose();
			}
		}
		++frame_counter_;
	});
}
