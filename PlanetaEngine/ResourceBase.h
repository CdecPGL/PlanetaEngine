﻿#pragma once
#include <memory>
#include "Object.h"
#include "LogUtility.h"
#include "ResourceReferencer.h"

namespace planeta {
	class File;
	class FileAccessor;
	class ResourceBase : public Object {
	public:
		ResourceBase() = default;
		virtual ~ResourceBase();

		bool Create(const File& file, private_::ResourceManagerInternalAccessor& mgr_acsr);
		void Dispose();
		bool is_usable()const { return is_usable_; }
		size_t reference_conunt()const;
	protected:
		virtual bool _Create(const File& file, ResourceReferencer& referencer) = 0;
		virtual void _Dispose() = 0;
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