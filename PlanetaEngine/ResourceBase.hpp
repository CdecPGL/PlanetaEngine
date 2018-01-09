#pragma once
#include <memory>
#include "Object.h"
#include "LogUtility.h"
#include "ResourceReferencer.h"

namespace planeta {
	class File;
	class JsonFile;
	class ResourceBase : public Object {
	public:
		ResourceBase() = default;
		virtual ~ResourceBase();

		bool Load(const File& file, const JsonFile& metadata, private_::ResourceManagerInternalAccessor& mgr_acsr);
		void Dispose();
		bool is_usable()const { return is_usable_; }
		size_t reference_conunt()const;
	protected:
		virtual bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) = 0;
		virtual void OnDisposed() = 0;
	private:
		ResourceBase(const ResourceBase&) = delete;
		ResourceBase(ResourceBase&&) = delete;
		ResourceBase& operator=(const ResourceBase&) = delete;
		ResourceBase& operator=(ResourceBase&&) = delete;
		bool is_usable_ = false; //使用可能か
		std::vector<std::shared_ptr<ResourceBase>> reference_resources; //参照しているリソース
		void ClearReference();
	};
}