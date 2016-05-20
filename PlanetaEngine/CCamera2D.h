#pragma once

#include "GameObjectSystemComponent.h"
#include "Vector2D.h"

namespace planeta_engine {
	class CTransform2D;
	class CCamera2D final: public core::GameObjectSystemComponent {
	public:
		//Šg‘å—¦
		double expansion()const;
		void expansion(double e);
		CTransform2D& transform2d();

		Vector2Dd CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& gui_space_pos)const;
		Vector2Di CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& gameobject_space_pos)const;
	private:
		double expansion_ = 1.0;

		utility::NonOwingPointer<CTransform2D> transform2d_;
		virtual bool OnInitialized() override;
		virtual bool OnActivated() override;
		virtual bool OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CCamera2D);
}
