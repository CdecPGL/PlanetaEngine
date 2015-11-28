#include "Component.h"
#include "GameObject.h"

namespace planeta_engine{
	namespace game{

		

		bool Component::Initialize() {
			if (Initialize_()) {
				is_valied_ = true;
				return true;
			} else {
				return false;
			}
		}

		void Component::Finalize() {
			Finalize_();
			is_valied_ = false;
		}

		bool Component::Activate() {
			if (is_active_) { return true; }
			if (Activated_()) {
				is_active_ = true;
				return true;
			} else {
				return false;
			}
		}

		bool Component::InActivate() {
			if (!is_active_) { return true; }
			if (InActivated_()) {
				is_active_ = false;
				return true;
			} else {
				return false;
			}
		}

	}
}
