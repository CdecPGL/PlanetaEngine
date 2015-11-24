#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

namespace planeta_engine {
	namespace core {
		/*�Փ˃}�g���b�N�X�B�����l��false*/
		class CollisionGroupMatrix {
		public:
			//�w�肵���O���[�vID�̃y�A���Փˉ\���m�F
			inline bool IsCollision(int col_group_id_1, int col_group_id_2)const {
				auto idp = std::minmax(col_group_id_1, col_group_id_2);
				assert(idp.first < (int)collision_matrix_.size());
				assert(idp.second - idp.first < (int)collision_matrix_[idp.first].size());
				return collision_matrix_[idp.first][idp.second - idp.first];
			}
			//�Փ˃O���[�v���Ɋ��蓖�Ă�ꂽID���擾(-1�Ŋ��蓖�Ă��ĂȂ�)
			int GetCollisionGroupID(const std::string& col_group_name)const;
			//�Փ˃O���[�v��ǉ�
			void AddCollisionGroup(const std::string& col_group_name);
			//�Փ˃O���[�v���܂Ƃ߂Ēǉ�
			void AddCollisionGroups(const std::vector<std::string>& col_group_names);
			//�O���[�v���ŏՓ˃}�g���b�N�X��ݒ�
			bool SetCollisionFlag(const std::string& col_group_name_1, const std::string& col_group_name_2, bool flag);
		private:
			std::unordered_map<std::string, int> collision_group_id_map_;
			std::vector<std::vector<bool>> collision_matrix_;
		};
	}
}