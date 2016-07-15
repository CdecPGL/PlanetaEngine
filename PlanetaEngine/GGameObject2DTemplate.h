#pragma once

#include "GameObject.h"
#include "JGameObject2D.h"

namespace planeta {
	template<class... GOI>
	class GGameObject2DTemplate : public GameObject<JGameObject2D, GOI...>{
	public:
		CTransform2D& transform2d()override { return *transform2d_; }
	protected:
		bool OnInitialized()override {
			transform2d_.reset(CreateAndAddComponent<CTransform2D>());
			return true;
		}
	private:
		util::NonOwingPointer<CTransform2D> transform2d_;
	};
}
