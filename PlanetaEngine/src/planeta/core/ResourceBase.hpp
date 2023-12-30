#pragma once

#include <memory>

#include "object.hpp"
#include "ResourceReferencer.hpp"

namespace plnt {
	class file;
	class json_file;

	class resource_base : public object {
	public:
		resource_base() = default;
		resource_base(const resource_base &) = delete;
		resource_base(resource_base &&) = delete;
		~resource_base() override;
		resource_base &operator=(const resource_base &) = delete;
		resource_base &operator=(resource_base &&) = delete;

		bool load(const file &file, const json_file &metadata, private_::resource_manager_internal_accessor &mgr_accessor);
		void dispose();
		[[nodiscard]] bool is_usable() const { return is_usable_; }
		[[nodiscard]] size_t reference_count() const;

	protected:
		virtual bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) = 0;
		virtual void on_disposed() = 0;

	private:
		bool is_usable_ = false; //使用可能か
		std::vector<std::shared_ptr<resource_base>> reference_resources_; //参照しているリソース
		void clear_reference();
	};
}
