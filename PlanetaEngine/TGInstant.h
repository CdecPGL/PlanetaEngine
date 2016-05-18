#pragma once

#include "TGameObjectOperation.h"

namespace planeta_engine {
	class TGInstant final : public TGameObjectOperation {
	public:
		void SetExcuteFunction(const std::function<void()>& func) { function_ = func; }
	private:
		std::function<void()> function_;
		void Update() override;
	};
	PE_REGISTER_TASK(TGInstant);
}
