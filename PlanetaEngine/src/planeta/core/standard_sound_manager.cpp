#include <unordered_map>

#include "standard_sound_manager.hpp"

#include <ranges>

#include "bgm_controller.hpp"
#include "sound_effect_controller.hpp"
#include "planeta/buildin/RMusic.hpp"
#include "planeta/buildin/RSound.hpp"
#include "log_utility.hpp"

namespace plnt::private_ {
	class standard_sound_manager::impl {
	public:
		std::shared_ptr<bgm_controller> bgm_controller;
		std::unordered_map<int, std::shared_ptr<sound_effect_controller>> se_controllers;
	};

	std::shared_ptr<i_bgm_controller> standard_sound_manager::get_bgm_controller(
		const std::shared_ptr<resource_base> &music_resource) {
		auto m_res = std::dynamic_pointer_cast<RMusic>(music_resource);
		if (m_res == nullptr) {
			// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
			PE_LOG_ERROR("Musicリソースでないリソースが渡されました。(", typeid(*music_resource).name(), ")");
			return nullptr;
		}
		if (impl_->bgm_controller) {
			if (impl_->bgm_controller->resource()->GetHandle() == m_res->GetHandle()) { return impl_->bgm_controller; }
			auto nbr = std::make_shared<bgm_controller>(m_res);
			impl_->bgm_controller = nbr;
			return nbr;
		}
		auto nbr = std::make_shared<bgm_controller>(m_res);
		impl_->bgm_controller = nbr;
		return nbr;
	}


	std::shared_ptr<i_sound_effect_controller> standard_sound_manager::get_sound_effect_controller(
		const std::shared_ptr<resource_base> &sound_resource) {
		auto s_res = std::dynamic_pointer_cast<RSound>(sound_resource);
		if (s_res == nullptr) {
			// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
			PE_LOG_ERROR("Soundリソースでないリソースが渡されました。(", typeid(*sound_resource).name(), ")");
			return nullptr;
		}
		int dx_handle = s_res->GetHandle();
		if (const auto it = impl_->se_controllers.find(dx_handle); it == impl_->se_controllers.end()) {
			auto new_controller = std::make_shared<sound_effect_controller>(s_res);
			impl_->se_controllers.emplace(dx_handle, new_controller);
			return new_controller;
		} else { return it->second; }
	}

	standard_sound_manager::standard_sound_manager() : impl_(std::make_unique<impl>()) {}

	void standard_sound_manager::update() { if (impl_->bgm_controller) { impl_->bgm_controller->update(); } }

	void standard_sound_manager::reset() {
		if (impl_->bgm_controller) {
			impl_->bgm_controller->dispose();
			impl_->bgm_controller.reset();
		}
		for (const auto &controller : impl_->se_controllers | std::views::values) { controller->dispose(); }
		impl_->se_controllers.clear();
	}

	bool standard_sound_manager::initialize() { return true; }

	void standard_sound_manager::finalize() {}

	standard_sound_manager::~standard_sound_manager() = default;
}
