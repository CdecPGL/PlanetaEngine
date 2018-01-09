
#include <unordered_map>
#include "StandardSoundManager.hpp"
#include "BGMController.hpp"
#include "SoundEffectController.hpp"
#include "RMusic.hpp"
#include "RSound.hpp"
#include "LogUtility.hpp"

namespace planeta {
	namespace private_ {

		class StandardSoundManager::Impl_ {
		public:
			std::shared_ptr<BGMController> bgm_controller_;
			std::unordered_map<int, std::shared_ptr<SoundEffectController>> se_controllers_;
		};

		std::shared_ptr<IBGMController> StandardSoundManager::GetBGMController(const std::shared_ptr<ResourceBase>& music_resource) {
			auto m_res = std::dynamic_pointer_cast<RMusic>(music_resource);
			if (m_res == nullptr) {
				PE_LOG_ERROR("Musicリソースでないリソースが渡されました。(", typeid(*music_resource).name(), ")");
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


		std::shared_ptr<ISoundEffectController> StandardSoundManager::GetSoundEffectController(const std::shared_ptr<ResourceBase>& sound_resource) {
			auto s_res = std::dynamic_pointer_cast<RSound>(sound_resource);
			if (s_res == nullptr) {
				PE_LOG_ERROR("Soundリソースでないリソースが渡されました。(", typeid(*sound_resource).name(),")");
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

		StandardSoundManager::StandardSoundManager() :impl_(std::make_unique<Impl_>()) {

		}

		void StandardSoundManager::Update() {
			if (impl_->bgm_controller_) { impl_->bgm_controller_->Update(); }
		}

		void StandardSoundManager::Reset() {
			if (impl_->bgm_controller_) {
				impl_->bgm_controller_->Dispose();
				impl_->bgm_controller_.reset();
			}
			for (auto& s : impl_->se_controllers_) {
				s.second->Dispose();
			}
			impl_->se_controllers_.clear();
		}

		bool StandardSoundManager::Initialize() {
			return true;
		}

		void StandardSoundManager::Finalize() {}

		StandardSoundManager::~StandardSoundManager() = default;
	}
}