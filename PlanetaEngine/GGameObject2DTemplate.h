#pragma once

#include "GameObject.h"
#include "JGameObject2D.h"

namespace planeta {
	template<class... GOI>
	class GGameObject2DTemplate : public GameObject<JGameObject2D, GOI...>{
	public:
		CTransform2D& transform2d()override { return *transform2d_; }
	protected:
		void AddComponentsProc(GOComponentAdder& com_adder) {
			transform2d_.reset(com_adder.CreateAndAddComponent<CTransform2D>());
		}
	private:
		util::NonOwingPointer<CTransform2D> transform2d_;
	};
}
