#include "CCamera2D.h"
#include "GameObjectDrawSystem.h"
#include "SceneData.h"
#include "SystemLog.h"
#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "DxLib.h"

namespace planeta {
	bool  CCamera2D::GetOtherComponentProc(const GOComponentGetter& com_getter) {
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	bool CCamera2D::OnInitialized() {
		if (!transform2d_) {
			PE_LOG_WARNING("CTransform2Dが取得できませんでした。");
			return false;
		}
		return scene_data_ref().gameobject_draw_system.RegisterCamera(std::static_pointer_cast<CCamera2D>(shared_from_this()));
	}

	bool CCamera2D::OnActivated() {
		return true;
	}

	bool CCamera2D::OnInactivated() {
		PE_LOG_WARNING("カメラ2Dコンポーネントは無効化に対応していません。");
		return true;
	}

	void CCamera2D::OnFinalized() noexcept {
		scene_data_ref().gameobject_draw_system.RemoveCamera(this);
	}

	CTransform2D& CCamera2D::transform2d() {
		return *transform2d_;
	}

	double CCamera2D::expansion() const {
		return expansion_;
	}

	void CCamera2D::expansion(double s) {
		expansion_ = s;
	}

	Vector2Dd CCamera2D::CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& ui_pos)const {
		VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
		return Vector2Dd(gov.x, gov.y);
	}

	Vector2Di CCamera2D::CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& game_object_pos)const {
		VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
		return Vector2Di((int)uiv.x, (int)uiv.y);
	}

}