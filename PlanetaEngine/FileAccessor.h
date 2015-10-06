#pragma once
#include <memory>
#include "Object.h"

/*–¢ŽÀ‘•*/
namespace planeta_engine {
	namespace file_system {
		class File;
		class FileAccessor : public core::Object {
		public:
			FileAccessor() = default;
			virtual ~FileAccessor() = 0 {};
			virtual size_t GetFileize()const = 0;
			virtual bool Read(size_t address, char* ptr, size_t size) = 0;
			template<typename T>
			bool Read(size_t address,T* ptr) { 
				return Read(address, reinterpret_cast<char*>(ptr), sizeof(T));
			}
			std::shared_ptr<File> GetAllData();

		private:
			
		};
	}
}