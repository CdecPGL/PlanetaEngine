﻿#pragma once
#include <memory>
#include "Object.h"
#include "SystemLog.h"

namespace planeta {
	class File;
	class FileAccessor;
	namespace private_ {
		class ResourceBase : public Object{
		public:
			ResourceBase() = default;
			virtual ~ResourceBase() { if (is_usable_) { PE_LOG_ERROR("リソースの解放が行われていません。(", typeid(*this).name(), ")"); } };
			bool Create(const std::shared_ptr<const File>& file) {
				if (is_usable_) { return false; }
				if (_Create(file)) {
					is_usable_ = true;
					return true;
				}
				else { return false; }
			}
			void Dispose() {
				if (is_usable_) { 
					_Dispose();
					is_usable_ = false;
				}
			}
			bool is_usable()const { return is_usable_; }
		protected:
			virtual bool _Create(const std::shared_ptr<const File>& file) = 0;
			virtual void _Dispose() = 0;
		private:
			ResourceBase(const ResourceBase&) = delete;
			ResourceBase(ResourceBase&&) = delete;
			ResourceBase& operator=(const ResourceBase&) = delete;
			ResourceBase& operator=(ResourceBase&&) = delete;
			bool is_usable_ = false; //使用可能か
		};
	}
}