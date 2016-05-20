#include "CollisionWorld.h"
#include "boost/next_prior.hpp"
#include "IGameObject.h"
#include "CollisionDetectFunctions.h"
#include "CCollider2D.h"
#include "CGround2D.h"
#include "EACollisionWithCollider2D.h"
#include "EACollisionWithGround2D.h"
#include "CollisionGroupMatrix.h"
#include "SystemLog.h"
#include "CTransform2D.h"
#include "Collider2DData.h"

namespace planeta_engine{
	namespace core {
		struct CollisionWorld::CCollider2DResistData_ {
			core::Collider2DData collider2d_data; //�R���C�_�[�f�[�^
			std::unordered_map<std::string, CollisionGroupType>::iterator group_iterator_at_collision_groups; //�R���W�����O���[�v�̃O���[�v���X�g���ł̃C�e���[�^
			CollisionGroupType::iterator iterator_at_collision_group; //�R���W�����O���[�v���ł̃C�e���[�^
			bool collision_with_ground_flag; //�n�`�Ƃ̏Փ˃t���O
			CollisionWithGroundListType::iterator iterator_at_collision_with_ground_list; //�n�`�ƏՓ˂���ꍇ�́A�n�`�Փ˃��X�g���ł̃C�e���[�^
		};

		CollisionWorld::CollisionWorld() = default;
		CollisionWorld::~CollisionWorld() = default;

		void CollisionWorld::Update()
		{
			
		}

		void CollisionWorld::ProcessCollisionInAGroup(CollisionGroupType& group, CollisionEventQue& collision_event_que)const {
			for (auto ccc_it = group.begin(); ccc_it != group.end(); ++ccc_it) {
				for (auto ccc_it2 = boost::next(ccc_it); ccc_it2 != group.end(); ++ccc_it2) {
					if ((*ccc_it).get().collider2d.DetectCollision((*ccc_it2).get().collider2d)) {
						//�Փ˂��Ă�����Փ˃C�x���g���z���_�[�ɒǉ�
						EACollisionWithCollider2D cea0((*ccc_it2).get().game_object);
						collision_event_que.push_back([eve = (*ccc_it).get().collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().game_object);
						collision_event_que.push_back([eve = (*ccc_it2).get().collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
		}

		void CollisionWorld::ProcessCollisionBetweenTwoGroups(CollisionGroupType& group1, CollisionGroupType& group2, CollisionEventQue& collision_event_que)const {
			for (auto ccc_it = group1.begin(); ccc_it != group1.end(); ++ccc_it) {
				for (auto ccc_it2 = group2.begin(); ccc_it2 != group2.end(); ++ccc_it2) {
					if ((*ccc_it).get().collider2d.DetectCollision((*ccc_it2).get().collider2d)) {
						//�Փ˂��Ă�����Փ˃C�x���g���z���_�[�ɒǉ�
						EACollisionWithCollider2D cea0((*ccc_it2).get().game_object);
						collision_event_que.push_back([eve = (*ccc_it).get().collide_with_collider_event_evoker, arg = cea0]() {eve(arg); });
						EACollisionWithCollider2D cea1((*ccc_it).get().game_object);
						collision_event_que.push_back([eve = (*ccc_it2).get().collide_with_collider_event_evoker, arg = cea1]() {eve(arg); });
					}
				}
			}
		}

		void CollisionWorld::ProcessCollisionWithGround(CollisionEventQue& collision_event_que)const {
			for (const auto& col_com : collision_with_ground_list_) {
				if (col_com.get().collider2d.DetectCollision(col_com.get().transform2d.ground())) {
					//�Փ˂��Ă�����n�`�Փ˃C�x���g���z���_�[�ɒǉ�
					EACollisionWithGround2D cwgea;
					collision_event_que.push_back([eve = col_com.get().collide_with_ground_event_evoker, arg = cwgea]() {eve(arg); });
				}
			}
		}

		bool CollisionWorld::Resist(const core::Collider2DData collider_data) {
			std::unique_ptr<CCollider2DResistData_> ccrd = std::unique_ptr<CCollider2DResistData_>(new CCollider2DResistData_{ collider_data, });
			const Collider2DData& col_dat = ccrd->collider2d_data;
			CCollider2D& ccol = col_dat.collider2d;
			std::string group_name = ccol.collision_group();
			//�Փ˃O���[�v�m�F
			auto it = collision_groupes_.find(ccol.collision_group());
			if (it == collision_groupes_.end()) {
				PE_LOG_WARNING("���݂��Ȃ��Փ˃O���[�v", ccol.collision_group(), "���w�肳�ꂽ���߃R���C�_�[��o�^�ł��܂���ł����B");
				return false; 
			}
			//�Փ˃O���[�v�ɓo�^
			ccrd->group_iterator_at_collision_groups = it;
			it->second.push_front(col_dat);
			ccrd->iterator_at_collision_group = it->second.begin();

			//�n�`�Փ˃R���C�_�[���X�g�ɓo�^
			ccrd->collision_with_ground_flag = ccol.collide_with_ground_flag();
			if (ccrd->collision_with_ground_flag) {
				collision_with_ground_list_.push_front(col_dat);
				ccrd->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			}
			//�o�^����o�^
			collider_resist_data_map_.emplace(&ccol, std::move(ccrd));
			return true;
		}

		bool CollisionWorld::Remove(const CCollider2D* col_com_ptr) {
			auto it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (it == collider_resist_data_map_.end()) {//�o�^����Ă��Ȃ��R���C�_�[
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���݂��Ȃ��R���C�_�[���w�肳��܂����B");
				assert(false);
				return false;
			} 
			//�n�`�Փ˃R���C�_�[���X�g���珜��
			if (it->second->collision_with_ground_flag) {
				collision_with_ground_list_.erase(it->second->iterator_at_collision_with_ground_list);
			}
			//�Փ˃O���[�v���珜��
			it->second->group_iterator_at_collision_groups->second.erase(it->second->iterator_at_collision_group);
			//�o�^��񂩂珜��
			collider_resist_data_map_.erase(it);
			return true;
		}

		bool CollisionWorld::ChangeCollisionGroup(const CCollider2D* col_com_ptr, const std::string& group_name) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //�o�^����Ă��Ȃ��R���C�_�[
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���݂��Ȃ��R���C�_�[���w�肳��܂����B");
				assert(false);
				return false;
			}
			if (resist_data_it->second->group_iterator_at_collision_groups->first == group_name) { return true; } //�ύX�̕K�v�͂Ȃ�
			auto group_it = collision_groupes_.find(group_name);
			if (group_it == collision_groupes_.end()) { //�w�肳�ꂽ�O���[�v�����݂��Ȃ�
				debug::SystemLog::instance().Log(debug::LogLevel::Warning, __FUNCTION__, "���݂��Ȃ��Փ˃O���[�v", group_name, "���w�肳��܂����B");
				return false;
			}
			//���݂̏Փ˃O���[�v���珜��
			resist_data_it->second->group_iterator_at_collision_groups->second.erase(resist_data_it->second->iterator_at_collision_group);
			//�V�����Փ˃O���[�v�ɓo�^
			resist_data_it->second->group_iterator_at_collision_groups = group_it;
			group_it->second.push_front(resist_data_it->second->collider2d_data);
			resist_data_it->second->iterator_at_collision_group = group_it->second.begin();
			return true;
		}

		bool CollisionWorld::ChangeCollisionWithGroundFlag(const CCollider2D* col_com_ptr, bool flag) {
			auto resist_data_it = collider_resist_data_map_.find(const_cast<CCollider2D*>(col_com_ptr));
			if (resist_data_it == collider_resist_data_map_.end()) { //�o�^����Ă��Ȃ��R���C�_�[
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "���݂��Ȃ��R���C�_�[���w�肳��܂����B");
				assert(false);
				return false;
			}

			if (resist_data_it->second->collision_with_ground_flag == flag) { return true; } //�ύX�͕K�v�Ȃ�
			resist_data_it->second->collision_with_ground_flag = flag;
			if (flag) { //�n�`�Փ˃R���C�_�[���X�g�ɓo�^
				collision_with_ground_list_.push_front(resist_data_it->second->collider2d_data);
				resist_data_it->second->iterator_at_collision_with_ground_list = collision_with_ground_list_.begin();
			} else { //�n�`�Փ˃R���C�_�[���X�g���珜��
				collision_with_ground_list_.erase(resist_data_it->second->iterator_at_collision_with_ground_list);
			}
			return true;
		}

		void CollisionWorld::SetCollisionGroupMatrix(const std::shared_ptr<const core::CollisionGroupMatrix>& col_matrix) { 
			auto collision_group_list = std::move(col_matrix->GetCollisionGroupList());
			for (const auto& group_name : collision_group_list) {
				collision_groupes_.emplace(group_name, CollisionGroupType());
			}

			auto collisionable_group_pair_list = std::move(col_matrix->GetCollisionableGroupPairList());
			collide_group_pair_list_.reserve(collisionable_group_pair_list.size());
			for (const auto& collisionable_group_pair : collisionable_group_pair_list) {
				auto it1 = collision_groupes_.find(collisionable_group_pair.first);
				assert(it1 != collision_groupes_.end());
				auto it2 = collision_groupes_.find(collisionable_group_pair.second);
				assert(it2 != collision_groupes_.end());
				collide_group_pair_list_.push_back(std::make_pair(it1, it2));
			}
		}

		void CollisionWorld::RemoveAll() {
			for (auto& group : collision_groupes_) {
				group.second.clear();
			}
			collide_group_pair_list_.clear();
			collider_resist_data_map_.clear();
		}

		void CollisionWorld::ExcuteCollisionDetection() {
			CollisionEventQue col_eve_que; //�Փ˃C�x���gQue
			//�R���C�_�[���m
			for (const auto& gp : collide_group_pair_list_) {
				if (gp.first == gp.second) { ProcessCollisionInAGroup(gp.first->second, col_eve_que); } //����O���[�v�̏ꍇ
				else { ProcessCollisionBetweenTwoGroups(gp.first->second, gp.second->second, col_eve_que); } //�Ⴄ�O���[�v�̏ꍇ
			}
			//�R���C�_�[�Ƃ��̏����n�`
			ProcessCollisionWithGround(col_eve_que);
			//�Փ˃C�x���g�̓`�B
			for (auto& eve : col_eve_que) {
				eve();
			}
		}

	}
}

