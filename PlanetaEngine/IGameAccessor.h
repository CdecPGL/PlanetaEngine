#pragma once

#include <string>
#include <memory>
#include <list>

namespace planeta_engine {
	namespace core {
		class ISceneManagerAccessor;
		class KeyInputManager;
		class ResourceManager;
		class CollisionGroupMatrix;
		class Screen;
		class IGameAccessor {
		public:
			virtual ~IGameAccessor() = default;
			virtual core::ISceneManagerAccessor& scene_manager() = 0;
			virtual core::KeyInputManager& key_input_manager() = 0;
			virtual core::ResourceManager& resource_manager() = 0;
			virtual std::shared_ptr<const CollisionGroupMatrix> GetCollisionGroupMatrix()const = 0;
			virtual const std::list<std::string>& GetTaskKeyPositionList()const = 0;
			virtual Screen& screen() = 0;
		};
	}
}