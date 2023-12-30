#include <unordered_map>
#include "StandardSoundManager.hpp"
#include "bgm_controller.hpp"
#include "SoundEffectController.hpp"
#include "planeta/buildin/RMusic.hpp"
#include "planeta/buildin/RSound.hpp"
#include "log_utility.hpp"

namespace plnt {
	namespace private_ {
		class StandardSoundManager::Impl_ {
		public:
			std::shared_ptr<bgm_controller> bgm_controller_;
			std::unordered_map<int, std::shared_ptr<sound_effect_controller>> se_controllers_;
		};

		std::shared_ptr<i_bgm_controller> StandardSoundManager::get_bgm_controller(
			const std::shared_ptr<resource_base> &music_resource) {
			auto m_res = std::dynamic_pointer_cast<RMusic>(music_resource);
			if (m_res == nullptr) {
				PE_LOG_ERROR("Musicリソースでないリソースが渡されました。(", typeid(*music_resource).name(), ")");
				return nullptr;
			}
			if (impl_->bgm_controller_) {
				if (impl_->bgm_controller_->resource()->GetHandle() == m_res->GetHandle()) {
					return impl_->bgm_controller_;
				} else {
					auto nbr = std::make_shared<bgm_controller>(m_res);
					impl_->bgm_controller_ = nbr;
					return nbr;
				}
			} else {
				auto nbr = std::make_shared<bgm_controller>(m_res);
				impl_->bgm_controller_ = nbr;
				return nbr;
			}
		}


		std::shared_ptr<i_sound_effect_controller> StandardSoundManager::get_sound_effect_controller(
			const std::shared_ptr<resource_base> &sound_resource) {
			auto s_res = std::dynamic_pointer_cast<RSound>(sound_resource);
			if (s_res == nullptr) {
				PE_LOG_ERROR("Soundリソースでないリソースが渡されました。(", typeid(*sound_resource).name(), ")");
				return nullptr;
			}
			int dx_handle = s_res->GetHandle();
			auto it = impl_->se_controllers_.find(dx_handle);
			if (it == impl_->se_controllers_.end()) {
				auto new_se_ctlr = std::make_shared<sound_effect_controller>(s_res);
				impl_->se_controllers_.emplace(dx_handle, new_se_ctlr);
				return new_se_ctlr;
			} else { return it->second; }
		}

		StandardSoundManager::StandardSoundManager() : impl_(std::make_unique<Impl_>()) { }

		void StandardSoundManager::update() { if (impl_->bgm_controller_) { impl_->bgm_controller_->update(); } }

		void StandardSoundManager::reset() {
			if (impl_->bgm_controller_) {
				impl_->bgm_controller_->dispose();
				impl_->bgm_controller_.reset();
			}
			for (auto &s : impl_->se_controllers_) { s.second->dispose(); }
			impl_->se_controllers_.clear();
		}

		bool StandardSoundManager::initialize() { return true; }

		void StandardSoundManager::finalize() { }

		StandardSoundManager::~StandardSoundManager() = default;
	}
}
