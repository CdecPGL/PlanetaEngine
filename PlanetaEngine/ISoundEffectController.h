#pragma once

namespace planeta {
	namespace private_ {
		class ISoundEffectController {
		public:
			virtual ~ISoundEffectController() = default;
			virtual bool Play() = 0;
			virtual bool is_valid()const = 0;
		};
	}
}
