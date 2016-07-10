
#include <unordered_map>
#include "SoundManager.h"
#include "BGMController.h"
#include "SoundEffectController.h"
#include "RMusic.h"
#include "RSound.h"
#include "SystemLog.h"

namespace planeta {
	namespace core {

		class SoundManager::Impl_ {
		public:
			std::shared_ptr<BGMController> bgm_controller_;
			std::unordered_map<int, std::shared_ptr<SoundEffectController>> se_controllers_;
		};

		std::shared_ptr<IBGMController> SoundManager::GetBGMController(const std::shared_ptr<core::ResourceBase>& music_resource) {
			auto m_res = std::dynamic_pointer_cast<RMusic>(music_resource);
			if (m_res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("Musicリソースでないリソースが渡されました。(") + music_resource->GetType().name() + ")", __FUNCTION__);
				return nullptr;
			}
			if (impl_->bgm_controller_) {
				if (impl_->bgm_controller_->resource()->GetHandle() == m_res->GetHandle()) {
					return impl_->bgm_controller_;
				} else {
					auto nbr = std::make_shared<BGMController>(m_res);
					impl_->bgm_controller_ = nbr;
					return nbr;
				}
			} else {
				auto nbr = std::make_shared<BGMController>(m_res);
				impl_->bgm_controller_ = nbr;
				return nbr;
			}
		}


		std::shared_ptr<ISoundEffectController> SoundManager::GetSoundEffectController(const std::shared_ptr<core::ResourceBase>& sound_resource) {
			auto s_res = std::dynamic_pointer_cast<RSound>(sound_resource);
			if (s_res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("Soundリソースでないリソースが渡されました。(") + sound_resource->GetType().name() + ")", __FUNCTION__);
				return nullptr;
			}
			int dx_handle = s_res->GetHandle();
			auto it = impl_->se_controllers_.find(dx_handle);
			if (it == impl_->se_controllers_.end()) {
				auto new_se_ctlr = std::make_shared<SoundEffectController>(s_res);
				impl_->se_controllers_.emplace(dx_handle, new_se_ctlr);
				return new_se_ctlr;
			} else {
				return it->second;
			}
		}

		SoundManager::SoundManager() :impl_(std::make_unique<Impl_>()) {

		}

		void SoundManager::Update() {
			if (impl_->bgm_controller_) { impl_->bgm_controller_->Update(); }
		}

		void SoundManager::Reset() {
			if (impl_->bgm_controller_) {
				impl_->bgm_controller_->Dispose();
				impl_->bgm_controller_.reset();
			}
			for (auto& s : impl_->se_controllers_) {
				s.second->Dispose();
			}
			impl_->se_controllers_.clear();
		}

		bool SoundManager::Initialize() {
			return true;
		}

		void SoundManager::Finalize() {}

		SoundManager::~SoundManager() = default;
	}
}