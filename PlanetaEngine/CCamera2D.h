#pragma once

#include "GameObjectSystemComponent.h"
#include "Vector2D.h"

namespace planeta {
	class CTransform2D;
	class CCamera2D final: public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CCamera2D);
	public:
		using Super = private_::GameObjectSystemComponent;
		//拡大率
		double expansion()const;
		void expansion(double e);
		CTransform2D& transform2d();

		Vector2Dd CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& gui_space_pos)const;
		Vector2Di CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& gameobject_space_pos)const;
	private:
		double expansion_ = 1.0;

		util::NonOwingPointer<CTransform2D> transform2d_;
		virtual bool GetOtherComponentsProc(const GOComponentGetter& com_getter) override;
		virtual bool OnInitialized()override;
		virtual bool OnActivated() override;
		virtual bool OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CCamera2D);
}
