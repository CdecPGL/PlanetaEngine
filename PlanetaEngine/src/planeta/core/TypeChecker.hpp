#pragma once

#include <memory>

namespace plnt {
	namespace util {
		template<typename Base,typename ArgType>
		bool CheckSharedPointerTypeOfBase(const std::shared_ptr<ArgType>& obj) { return std::dynamic_pointer_cast<Base>(obj) != nullptr; }
		template<typename Base, typename ArgType>
		bool CheckPointerTypeOfBase(ArgType* obj) { return dynamic_cast<Base*>(obj) != nullptr; }
	}
}