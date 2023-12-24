#pragma once

#include "planeta/PlanetaEngine.hpp"

class CPlayerInputPreserver;
class TPlayerInputSender : public plnt::Task {
public:
	using Super = plnt::Task;
	bool SetTarget(plnt::WeakPointer<plnt::IGameObject> game_object);
private:
	void Update()override;
	plnt::WeakPointer<CPlayerInputPreserver> target_;
};
PE_TASK_CLASS(TPlayerInputSender);