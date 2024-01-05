#include "c_disposer_when_effect_stop.hpp"

using namespace plnt;

void c_disposer_when_effect_stop::on_initialized() {
	super::on_initialized();
	const auto task = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
	auto eff_com = game_object().get_component<c_effect>();

	if (eff_com == nullptr) {
		PE_LOG_ERROR("エフェクトコンポーネントが設定されていません。");
		return;
	}
	task->set_execute_function([this, eff_com] {
		//作成された直後はまだ再生が開始されていない可能性があるので、一フレーム待ってから判定する。
		if (frame_counter_ > 0) { if (!eff_com->is_playing()) { game_object().dispose(); } }
		++frame_counter_;
	});
}
