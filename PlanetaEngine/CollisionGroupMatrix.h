#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace planeta {
	/*�Փ˃}�g���b�N�X�B�����l��false*/
	class CollisionGroupMatrix {
	public:
		//�Փ˃O���[�v��ǉ�
		void AddCollisionGroup(const std::string& col_group_name);
		//�Փ˃O���[�v���܂Ƃ߂Ēǉ�
		void AddCollisionGroups(const std::vector<std::string>& col_group_names);
		//�O���[�v�Ԃ̏Փˉۂ�ݒ�
		bool SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag);
		//�O���[�v�Ԃ̏Փˉۂ��擾(�ݒ肳��Ă��Ȃ��A���݂��Ȃ��ꍇ��false)
		bool GetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2)const;
		//�Փ˂���y�A�̃��X�g���擾
		std::vector<std::pair<std::string, std::string>> GetCollisionableGroupPairList()const;
		//�Փ˃O���[�v�̃��X�g���擾
		std::vector<std::string> GetCollisionGroupList()const;
	private:
		std::unordered_map<std::string, std::unordered_map<std::string, bool>> collision_matrix_;
	};
}