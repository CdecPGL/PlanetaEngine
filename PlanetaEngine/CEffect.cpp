#include "CEffect.h"
#include "CTransform2D.h"
#include "REffect.h"
#include "EffekseerForDXLib.h"
#include "ResourceManager.h"
#include "TGInstant.h"
#include "IGameObject.h"

namespace planeta {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CEffect::Impl_ {
	public:
		Impl_& operator=(const Impl_& obj);

		bool CreateEffectInstance();
		bool StartEffect();
		bool PauseEffect();
		bool ClearEffectInstance();

		bool SetResourceByID(const std::string& id);
		bool GetEffectExits()const;
		void ApplyTransformToEffect();

		void SetMyCTransform2D(const NonOwingPointer<CTransform2D>& com);
		
		bool roop_flag = false;
	private:
		NonOwingPointer<CTransform2D> my_c_transform_2d_;
		std::shared_ptr<REffect> reffect_;
		int effect_handle_ = -1;
	};

	CEffect::Impl_& CEffect::Impl_::operator=(const Impl_& obj) {
		reffect_ = obj.reffect_;
		roop_flag = obj.roop_flag;
		return *this;
	}

	bool CEffect::Impl_::CreateEffectInstance() {
		ClearEffectInstance();
		if (reffect_) {
			auto* eff_mgr = GetEffekseer3DManager();
			auto& trans = *my_c_transform_2d_;
			effect_handle_ = eff_mgr->Play(reffect_->effekseer_effect(), trans.position().x, trans.position().y, 0);
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
			return effect_handle_ >= 0;
		} else {
			return true;
		}
	}

	bool CEffect::Impl_::StartEffect() {
		if (reffect_) {
			auto* eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, false);
			eff_mgr->SetShown(effect_handle_, true);
		}
		return true;
	}

	bool CEffect::Impl_::PauseEffect() {
		if (reffect_) {
			auto* eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
		}
		return true;
	}

	bool CEffect::Impl_::ClearEffectInstance() {
		if (reffect_) {
			auto* eff_mgr = GetEffekseer3DManager();
			eff_mgr->StopEffect(effect_handle_);
		}
		return true;
	}

	bool CEffect::Impl_::SetResourceByID(const std::string& resource_id) {
		auto res = private_::ResourceManager::instance().GetResourceByID<REffect>(resource_id);
		if (res) {
			reffect_ = res;
			return true;
		} else {
			PE_LOG_ERROR("エフェクトリソースの設定に失敗しました。指定IDは\"", resource_id, "\"。");
			return false;
		}
	}

	bool CEffect::Impl_::GetEffectExits() const {
		if (effect_handle_ >= 0) {
			auto* eff_mgr = GetEffekseer3DManager();
			return eff_mgr->Exists(effect_handle_);
		} else {
			return false;
		}
	}

	void CEffect::Impl_::ApplyTransformToEffect() {
		auto& trans = *my_c_transform_2d_;
		auto* eff_mgr = GetEffekseer3DManager();
		eff_mgr->SetLocation(effect_handle_, trans.position().x, trans.position().y, 0);
		eff_mgr->SetRotation(effect_handle_, 0, 0, trans.rotation_rad());
		eff_mgr->SetScale(effect_handle_, trans.scale().x, trans.scale().y, (trans.scale().x + trans.scale().y) / 2);
	}

	void CEffect::Impl_::SetMyCTransform2D(const NonOwingPointer<CTransform2D>& com) {
		my_c_transform_2d_.reset(com);
	}

	//////////////////////////////////////////////////////////////////////////
	//CEffect
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CEffect) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CEffect, roop_flag)
			.PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(CEffect, resource_id)
			.PE_REFLECTABLE_CLASS_READONLY_PROPERTY(CEffect, is_playing)
			.DeepCopyTarget(&CEffect::impl_);
	}

	CEffect::CEffect() :impl_(std::make_unique<Impl_>()) {}

	CEffect::~CEffect() = default;

	bool CEffect::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		impl_->SetMyCTransform2D(com_getter.GetComponent<CTransform2D>());
		return true;
	}

	bool CEffect::OnInitialized() {
		if (!Super::OnInitialized()) { return false; }
		if (!impl_->ClearEffectInstance()) { return false; }
		auto task = game_object().CreateAndAttachTask<TGInstant>(TaskSlot::PreDrawUpdatePhase);
		task->SetExcuteFunction([this] {
			impl_->ApplyTransformToEffect();
			//エフェクトのループ確認
			if (impl_->roop_flag && !impl_->GetEffectExits()) {
				if (is_valied()) {
					if (!impl_->CreateEffectInstance()) { return; }
					if (is_active()) {
						impl_->StartEffect();
					}
				}
			}
			PE_LOG_MESSAGE("asdjiaosjdiajdiasjdiajsiod");
		});
		return true;
	}

	bool CEffect::OnActivated() {
		if (!Super::OnActivated()) { return false; }
		return impl_->StartEffect();
	}

	bool CEffect::OnInactivated() {
		impl_->PauseEffect();
		return Super::OnInactivated();
	}

	void CEffect::OnFinalized() noexcept {
		impl_->ClearEffectInstance();
		Super::OnFinalized();
	}

	void CEffect::resource_id(const std::string& resource_id) {
		if (!impl_->SetResourceByID(resource_id)) { return; }
		if (is_valied()) {
			if (!impl_->CreateEffectInstance()) { return; }
			if (is_active()) {
				impl_->StartEffect();
			}
		}
	}

	bool CEffect::is_playing() const {
		return impl_->GetEffectExits() && is_active();
	}

	void CEffect::roop_flag(bool f) {
		impl_->roop_flag = f;
	}

	bool CEffect::roop_flag() const {
		bool f = impl_->roop_flag;
		return f;
	}

}