#pragma warning(push)
#pragma warning(disable: 4100)
#include "EffekseerForDXLib.h"
#pragma warning(pop)

#include "planeta/core/Game.hpp"
#include "planeta/core/IResourceManager.hpp"
#include "planeta/core/StandardResourceManager.hpp"
#include "planeta/core/IGameObject.hpp"

#include "CEffect.hpp"
#include "CTransform2D.hpp"
#include "REffect.hpp"
#include "TInstant.hpp"

namespace plnt {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////

	class CEffect::Impl_ {
	public:
		Impl_ &operator=(const Impl_ &obj);

		bool CreateEffectInstance();
		bool StartEffect();
		bool PauseEffect();
		bool ClearEffectInstance();

		bool SetResourceByID(const std::string &id);
		bool GetEffectExits() const;
		void ApplyTransformToEffect();

		void SetMyCTransform2D(const NonOwingPointer<CTransform2D> &com);
		void DisconnectMyCTransformUpdatedEvent();
		void ConnectMyCTransformUpdatedEvent();

		bool roop_flag_ = false;
		bool auto_play_ = true;
		double expansion_ = 1.0;
		SignalConnection trans_update_eve_connection_;

	private:
		NonOwingPointer<CTransform2D> my_c_transform_2d_;
		std::shared_ptr<REffect> reffect_;
		int effect_handle_ = -1;
	};

	CEffect::Impl_ &CEffect::Impl_::operator=(const Impl_ &obj) {
		reffect_ = obj.reffect_;
		roop_flag_ = obj.roop_flag_;
		auto_play_ = obj.auto_play_;
		expansion_ = obj.expansion_;
		return *this;
	}

	bool CEffect::Impl_::CreateEffectInstance() {
		ClearEffectInstance();
		if (reffect_) {
			auto eff_mgr = GetEffekseer3DManager();
			auto &trans = *my_c_transform_2d_;
			effect_handle_ = eff_mgr->Play(reffect_->effekseer_effect(),
			                               static_cast<float>(trans.position().x),
			                               static_cast<float>(trans.position().y), 0);
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
			return effect_handle_ >= 0;
		} else { return true; }
	}

	bool CEffect::Impl_::StartEffect() {
		if (reffect_) {
			auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, false);
			eff_mgr->SetShown(effect_handle_, true);
		}
		return true;
	}

	bool CEffect::Impl_::PauseEffect() {
		if (reffect_) {
			auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->SetPaused(effect_handle_, true);
			eff_mgr->SetShown(effect_handle_, false);
		}
		return true;
	}

	bool CEffect::Impl_::ClearEffectInstance() {
		if (reffect_) {
			auto eff_mgr = GetEffekseer3DManager();
			eff_mgr->StopEffect(effect_handle_);
		}
		return true;
	}

	bool CEffect::Impl_::SetResourceByID(const std::string &resource_id) {
		auto res = Game::instance().resource_manager()->GetResourceByID<REffect>(resource_id);
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
			auto eff_mgr = GetEffekseer3DManager();
			return eff_mgr->Exists(effect_handle_);
		} else { return false; }
	}

	void CEffect::Impl_::ApplyTransformToEffect() {
		auto &trans = *my_c_transform_2d_;
		auto eff_mgr = GetEffekseer3DManager();
		eff_mgr->SetLocation(effect_handle_,
		                     static_cast<float>(trans.position().x), static_cast<float>(trans.position().y), 0);
		eff_mgr->SetRotation(effect_handle_, 0, 0,
		                     static_cast<float>(trans.rotation_rad()));
		auto scl = trans.scale();
		scl *= expansion_; //拡大率適用
		eff_mgr->SetScale(effect_handle_,
		                  static_cast<float>(scl.x), static_cast<float>(scl.y),
		                  static_cast<float>((scl.x + scl.y) / 2));
	}

	void CEffect::Impl_::ConnectMyCTransformUpdatedEvent() {
		if (my_c_transform_2d_) {
			trans_update_eve_connection_.Disconnect();
			trans_update_eve_connection_ = my_c_transform_2d_->updated.ConnectFunction([this]() {
				ApplyTransformToEffect();
			});
		}
	}

	void CEffect::Impl_::DisconnectMyCTransformUpdatedEvent() { trans_update_eve_connection_.Disconnect(); }

	void CEffect::Impl_::SetMyCTransform2D(const NonOwingPointer<CTransform2D> &com) {
		my_c_transform_2d_.reset(com);
		ConnectMyCTransformUpdatedEvent();
	}

	//////////////////////////////////////////////////////////////////////////
	//CEffect
	//////////////////////////////////////////////////////////////////////////

	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CEffect) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CEffect, roop_flag)
			.PE_REFLECTABLE_CLASS_PROPERTY(CEffect, auto_play)
			.PE_REFLECTABLE_CLASS_PROPERTY(CEffect, expansion)
			.PE_REFLECTABLE_CLASS_WRITEONLY_PROPERTY(CEffect, resource_id)
			.PE_REFLECTABLE_CLASS_READONLY_PROPERTY(CEffect, is_playing)
			.DeepCopyTarget(&CEffect::impl_);
	}

	CEffect::CEffect() : impl_(std::make_unique<Impl_>()) { }

	CEffect::~CEffect() = default;

	bool CEffect::GetOtherComponentsProc(const GOComponentGetter &com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		impl_->SetMyCTransform2D(com_getter.GetComponent<CTransform2D>());
		return true;
	}

	void CEffect::OnInitialized() {
		Super::OnInitialized();
		if (!impl_->GetEffectExits()) {
			if (!impl_->CreateEffectInstance()) {
				PE_LOG_ERROR("エフェクトインスタンスの作成に失敗しました。");
				return;
			}
		}
		auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreDrawUpdatePhase);
		task->SetExcuteFunction([this] {
			impl_->ApplyTransformToEffect();
			//エフェクトのループ確認
			if (impl_->roop_flag_ && !impl_->GetEffectExits()) {
				if (is_valied()) {
					if (!impl_->CreateEffectInstance()) { return; }
					if (is_active()) { impl_->StartEffect(); }
				}
			}
		});
	}

	void CEffect::OnActivated() {
		Super::OnActivated();
		impl_->ConnectMyCTransformUpdatedEvent();
		if (auto_play()) { if (!impl_->StartEffect()) { PE_LOG_ERROR("エフェクトの開始に失敗しました。"); } }
	}

	void CEffect::OnInactivated() {
		impl_->DisconnectMyCTransformUpdatedEvent();
		impl_->PauseEffect();
		return Super::OnInactivated();
	}

	void CEffect::OnFinalized() noexcept {
		impl_->ClearEffectInstance();
		Super::OnFinalized();
	}

	void CEffect::resource_id(const std::string &resource_id) {
		if (!impl_->SetResourceByID(resource_id)) { return; }
		if (is_valied()) {
			if (!impl_->CreateEffectInstance()) { return; }
			if (is_active() && auto_play()) { impl_->StartEffect(); }
		}
	}

	bool CEffect::is_playing() const { return impl_->GetEffectExits() && is_active(); }

	void CEffect::roop_flag(bool f) { impl_->roop_flag_ = f; }

	bool CEffect::roop_flag() const {
		bool f = impl_->roop_flag_;
		return f;
	}

	void CEffect::auto_play(bool f) { impl_->auto_play_ = f; }

	bool CEffect::auto_play() const { return impl_->auto_play_; }

	void CEffect::expansion(double e) { impl_->expansion_ = e; }

	double CEffect::expansion() const { return impl_->expansion_; }

	bool CEffect::Play() { return impl_->StartEffect(); }

	bool CEffect::Stop() { return impl_->PauseEffect(); }
}
