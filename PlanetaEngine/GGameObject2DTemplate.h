#pragma once

#include "GameObject.h"
#include "JGameObject2D.h"

namespace planeta {
	template<class... GOI>
	class GGameObject2DTemplate : public GameObject<JGameObject2D, GOI...>{
	public:
		CTransform2D& transform2d()override { return *transform2d_; }
		const CTransform2D& transform2d()const override { return *transform2d_; }
	protected:
		void AddComponentsProc(GOComponentAdder& com_adder) {
			Super::AddComponentsProc(com_adder);
			com_adder.CreateAndAddComponent<CTransform2D>();
		}
		bool OnInitialized(const GOComponentGetter& com_getter) {
			if (!Super::OnInitialized(com_getter)) { return false; }
			transform2d_.reset(com_getter.GetComponent<CTransform2D>());
			return true;
		}
	private:
		NonOwingPointer<CTransform2D> transform2d_;
	};
}
