#pragma once
#include <memory>
#include "ResourceBase.h"

namespace planeta_engine {
	namespace core {
		/*この関数を使ってリソースを作成することでリソースの破棄が自動で行われるようになる*/
		template<class Res>
		std::shared_ptr<Res> MakeResource() {
			static_assert(std::is_base_of<ResourceBase, Res>::value == true, "Res is not derived ResourceBase.");
			return std::move(std::shared_ptr<Res>(new Res(), [](Res* r)->void {r->Dispose(); delete r; }));
		}
	}
}