#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <functional>
#include "SceneModule.h"

namespace planeta_engine {
	class CCollider2D;
	class CollisionGroupMatrix;
	namespace core {
		struct Collider2DData;
	}
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	namespace core {
		class CollisionWorld final: public core::SceneModule
		{
		public:
			CollisionWorld();
			~CollisionWorld();
			void SetCollisionGroupMatrix(const std::shared_ptr<const CollisionGroupMatrix>& col_matrix);
			bool Resist(const core::Collider2DData collider_data); //�����͓����ŃR�s�[�����̂ňꎞ�I�u�W�F�N�g�ł悢�B
			bool Remove(const CCollider2D* col_com_ptr);
			bool ChangeCollisionGroup(const CCollider2D* col_com_ptr,const std::string& group_name);
			bool ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag);

			void ExcuteCollisionDetection();
		private:
			void Update()override final;
			void RemoveAll();
			using CollisionGroupType = std::list<std::reference_wrapper<const core::Collider2DData>>;
			using CollisionWithGroundListType = std::list<std::reference_wrapper<const core::Collider2DData>>;
			struct CCollider2DResistData_;
			std::unordered_map<CCollider2D*, std::unique_ptr<CCollider2DResistData_>> collider_resist_data_map_; //�R���C�_�[�̓o�^��񃊃X�g
			using CollisionGroupListType = std::unordered_map<std::string, CollisionGroupType>;
			CollisionGroupListType collision_groupes_;
			CollisionWithGroundListType collision_with_ground_list_; //�n�`�ƏՓ˂���R���C�_�[�̃��X�g

			std::vector<std::pair<CollisionGroupListType::iterator, CollisionGroupListType::iterator>> collide_group_pair_list_; //�Փ˂���O���[�v�y�A�̃��X�g

			using CollisionEventQue = std::vector<std::function<void()>>;
			void ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_holder)const; //�Q�̃O���[�v�Ԃł̏Փ˔���
			void ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_holder)const; //�O���[�v���ł̏Փ˔���
			void ProcessCollisionWithGround(CollisionEventQue& collision_event_holder)const; //�n�`�Ƃ̏Փ˔���
		};
	}
}
