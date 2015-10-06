#pragma once
#include <memory>
#include "ResourceBase.h"

namespace planeta_engine {
	namespace core {
		/*この関数を使ってリソースを作成することでリソースの破棄が自動で行われるようになる*/
		template<class Res>
		std::shared_ptr<Res> MakeResource() { return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; })); }
	}
}