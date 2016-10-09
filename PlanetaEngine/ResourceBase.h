#pragma once
#include <memory>
#include "Object.h"

namespace planeta {
	class File;
	class FileAccessor;
	namespace private_ {
		class ResourceBase : public Object{
		public:
			ResourceBase() = default;
			virtual ~ResourceBase();;

			bool Create(const File& file);
			void Dispose();
			bool is_usable()const { return is_usable_; }
		protected:
			virtual bool _Create(const File& file) = 0;
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