#include "TPlayerInputSender.hpp"
#include "CPlayerInputPreserver.hpp"

using namespace plnt;

bool TPlayerInputSender::SetTarget(plnt::WeakPointer<plnt::IGameObject> game_object) {
	auto gi_pc = game_object->GetComponent<CPlayerInputPreserver>();
	if (gi_pc) {
		if (target_) { target_->ResetInput(); }
		target_ = gi_pc;
		return true;
	} else {
		PE_LOG_ERROR("CPlayerInputPreserverを持たないゲームオブジェクトが指定されました。");
		return false;
	}
}

void TPlayerInputSender::Update() {
	using InputCode = CPlayerInputPreserver::InputCode;
	if (target_) {
		//キー入力伝達
		auto& kim = *Game::instance().input_manager();
		InputCode::Type input = 0;
		if (kim.KeyState(Key::LEFT) | kim.KeyState(Key::A)) {
			input |= InputCode::Left;
		} else if (kim.KeyState(Key::RIGHT) | kim.KeyState(Key::D)) {
			input |= InputCode::Right;
		}
		if (kim.KeyState(Key::UP) | kim.KeyState(Key::W)) {
			input |= InputCode::Up;
		} else if (kim.KeyState(Key::DOWN) | kim.KeyState(Key::S)) {
			input |= InputCode::Down;
		}
		if (kim.KeyState(Key::C)) {
			input |= InputCode::Jump;
		}
		if (kim.MouseButtonState(MouseButton::RIGHT)) {
			input |= InputCode::PhysicalAttack;
		}
		if (kim.MouseButtonState(MouseButton::LEFT)) {
			input |= InputCode::ShotAttack;
		}
		target_->UpdateInput(input);
		//カーソル位置伝達
		target_->UpdatePointer(kim.GetMousePointerPosition());
	}
}
