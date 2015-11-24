#pragma once
#include "GameProcess.h"
#include <memory>
#include <unordered_map>
#include <list>

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
	}
	namespace core {
		class CollisionGroupMatrix;
	}
	namespace system_processes {
		class CollisionDetectProcess : public game::GameProcess
		{
		public:
			GameProcess::GameProcess;
			~CollisionDetectProcess();
			void SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix) { collision_group_matrix_ = col_matrix; }
			void Resist(const std::shared_ptr<components::CircleColliderComponent>& col_com) { _circle_collision_component_holder.Resist(col_com); }
			void Resist(const std::shared_ptr<components::StraightLineColliderComponent>& col_com) { _straightline_collision_component_holder.Resist(col_com); }
			bool Remove(const std::shared_ptr<components::CircleColliderComponent>& col_com) { return _circle_collision_component_holder.Remove(col_com); }
			bool Remove(const std::shared_ptr<components::StraightLineColliderComponent>& col_com) { return _straightline_collision_component_holder.Remove(col_com); }
			const core::CollisionGroupMatrix& collision_group_matrix()const { assert(collision_group_matrix_ != nullptr); return *collision_group_matrix_; }
		private:
			void Update()override final;
			void RemoveAll() {
				_circle_collision_component_holder.RemoveAll();
				_straightline_collision_component_holder.RemoveAll();
			}
			template<class Com>
			class CollisionComponentHolder_ {
			public:
				void Update() {
					for (const auto& com : remove_list_) {
						auto it = collision_component_map_.find(com.get());
						collision_component_list_.erase(it->second);
						collision_component_map_.erase(it);
					}
					remove_list_.clear();
				}
				void Resist(const std::shared_ptr<Com>& col_com) {
					collision_component_list_.push_back(col_com);
					collision_component_map_.emplace(col_com.get(), --collision_component_list_.end());
				}
				bool Remove(const std::shared_ptr<Com>& col_com) {
					auto it = collision_component_map_.find(col_com.get());
					if (it == collision_component_map_.end()) { return false; }
					else {
						remove_list_.push_back(col_com);
						return true;
					}
				}
				void RemoveAll() {
					collision_component_list_.clear();
					collision_component_map_.clear();
				}
				std::list<std::shared_ptr<Com>>& GetCollisionComponentList() { return collision_component_list_; }
			private:
				std::list<std::shared_ptr<Com>> collision_component_list_;
				std::list<std::shared_ptr<Com>> remove_list_;
				std::unordered_map<Com*, typename std::list<std::shared_ptr<Com>>::iterator> collision_component_map_;
			};
			CollisionComponentHolder_<components::CircleColliderComponent> _circle_collision_component_holder;
			CollisionComponentHolder_<components::StraightLineColliderComponent> _straightline_collision_component_holder;
			std::shared_ptr<const core::CollisionGroupMatrix> collision_group_matrix_;

		};
	}
}
