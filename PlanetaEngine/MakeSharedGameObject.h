#pragma once

#include <memory>

namespace planeta_engine{
	namespace game{
		template<class C>
		std::shared_ptr<C> MakeShared(){ return std::make_shared<C>(); }
		template<class C,typename A>
		std::shared_ptr<C> MakeShared(A& arg){ return std::make_shared<C>(arg); }
		template<class C, typename A,typename B>
		std::shared_ptr<C> MakeShared(A& arg, B& arg2){ return std::make_shared<C>(arg, arg2); }
	}
}