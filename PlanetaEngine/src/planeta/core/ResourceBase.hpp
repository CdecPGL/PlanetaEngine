#pragma once
#include <memory>
#include "Object.hpp"
#include "log_utility.hpp"
#include "ResourceReferencer.hpp"

namespace plnt {
	class file;
	class json_file;

	class ResourceBase : public object {
	public:
		ResourceBase() = default;
		virtual ~ResourceBase();

		bool Load(const file &file, const json_file &metadata, private_::ResourceManagerInternalAccessor &mgr_acsr);
		void Dispose();
		bool is_usable() const { return is_usable_; }
		size_t reference_conunt() const;

	protected:
		virtual bool OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) = 0;
		virtual void OnDisposed() = 0;

	private:
		ResourceBase(const ResourceBase &) = delete;
		ResourceBase(ResourceBase &&) = delete;
		ResourceBase &operator=(const ResourceBase &) = delete;
		ResourceBase &operator=(ResourceBase &&) = delete;
		bool is_usable_ = false; //使用可能か
		std::vector<std::shared_ptr<ResourceBase>> reference_resources; //参照しているリソース
		void ClearReference();
	};
}
