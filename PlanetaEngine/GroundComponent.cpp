#include "GroundComponent.h"

namespace PlanetaEngine{
	namespace Game{
		namespace Components{
			GroundComponent::GroundComponent(GameObject& go,int id) :Component(go,id)
			{
			}


			GroundComponent::~GroundComponent()
			{
			}

			bool GroundComponent::Initialize()
			{
				return true;
			}

			bool GroundComponent::Activated()
			{
				return true;
			}

			void GroundComponent::Update()
			{
				
			}

			bool GroundComponent::InActivated()
			{
				return true;
			}

			void GroundComponent::Finalize() throw()
			{
				
			}

		}
	}
}
