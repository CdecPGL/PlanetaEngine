#include "GroundComponent.h"

namespace planeta_engine{
	namespace components {
		GroundComponent::GroundComponent()
		{
		}

		bool GroundComponent::OnInitialized()
		{
			return true;
		}

		bool GroundComponent::OnActivated()
		{
			return true;
		}

		void GroundComponent::OnUpdated()
		{

		}

		bool GroundComponent::OnInactivated()
		{
			return true;
		}

		void GroundComponent::OnFinalized() throw()
		{

		}

	}
}
