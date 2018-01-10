﻿#pragma once

#include <memory>

namespace plnt{
	namespace private_{
		namespace ObjectUtility{
			template<class C>
			std::shared_ptr<C> make_shared(){ return std::make_shared<C>(); }
			template<class C>
			std::unique_ptr<C> make_unique(){ return std::move(std::make_unique<C>()); }
		}
	}
}