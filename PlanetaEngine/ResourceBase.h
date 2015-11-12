#pragma once
#include <memory>
#include "Object.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {
		class File;
		class FileAccessor;
	}
	namespace core {
		class ResourceBase : public core::Object{
		public:
			ResourceBase() = default;
			virtual ~ResourceBase() { if (is_usable_) { debug::SystemLog::instance().LogError(std::string("リソースの解放が行われていません。(") + GetType().name() + ")", "ResourceBase::~ResourceBase"); } };
			bool Create(const std::shared_ptr<const file_system::File>& file) {
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
			virtual bool _Create(const std::shared_ptr<const file_system::File>& file) = 0;
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