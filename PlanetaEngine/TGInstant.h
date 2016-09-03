#pragma once

#include "TGameObjectOperation.h"

namespace planeta {
	class TGInstant final : public TGameObjectOperation {
	public:
		using Super = TGameObjectOperation;
		void SetExcuteFunction(const std::function<void()>& func) { function_ = func; }
	private:
		std::function<void()> function_;
		void Update() override;
	};
	PE_TASK_CLASS(TGInstant);
}
