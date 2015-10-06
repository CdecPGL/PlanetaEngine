#pragma once
#include <memory>
#include "Object.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace file_system {
		class File;
	}
	namespace core {
		class ResourceBase : public core::Object{
		public:
			ResourceBase() = default;
			virtual ~ResourceBase() { if (_is_usable) { debug::SystemLog::instance().LogError(std::string("リソースの解放が行われていません。(") + GetType().name() + ")", "ResourceBase::~ResourceBase"); } };
			bool Create(const std::shared_ptr<file_system::File>& file) {
				if (_is_usable) { return false; }
				if (_Create(file)) {
					_is_usable = true;
					return true;
				}
				else { return false; }
			}
			void Dispose() {
				if (_is_usable) { 
					_Dispose();
					_is_usable = false;
				}
			}
		protected:
			virtual bool _Create(const std::shared_ptr<file_system::File>& file) = 0;
			virtual void _Dispose() = 0;
		private:
			ResourceBase(const ResourceBase&) = delete;
			ResourceBase(ResourceBase&&) = delete;
			ResourceBase& operator=(const ResourceBase&) = delete;
			ResourceBase& operator=(ResourceBase&&) = delete;
			bool _is_usable = false; //使用可能か
		};
	}
}