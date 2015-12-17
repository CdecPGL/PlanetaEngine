#include "GameProcessKeyPositionDefinition.h"
#include "GameProcessSystemKeyPositions.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {

		GameProcessKeyPositionDefinition::GameProcessKeyPositionDefinition() {
			namespace sp_kp = gameprocess_sys_key_pos;
			std::list<std::string> default_gpkps = {
				sp_kp::GameObjectUpdatetProcessKeyPositionID,
				sp_kp::TransformApplyVelocityProcessKeyPositionID,
				sp_kp::CollisionDetectProcessKeyPositionID,
				sp_kp::GameObjectDrawProcessKeyPositionID,
				sp_kp::GUIUpdateProcessKeyPositionID,
				sp_kp::GUIDrawProcessKeyPositionID,
			};
			for (const auto& id : default_gpkps) {
				key_position_id_list_.push_back(id);
				id_it_map_.emplace(std::make_pair(id, --key_position_id_list_.end()));
			}
		}

		bool GameProcessKeyPositionDefinition::AddKeyPosition(const std::string& id, InsertPosIndication ins_ind, const std::string& pos_id) {
			auto map_it = id_it_map_.find(pos_id);
			if (map_it == id_it_map_.end()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__,"存在しないキーポジションID\"", pos_id, "\"が指定されました。");
				return false;
			}
			auto list_it = map_it->second;
			switch (ins_ind) {
			case planeta_engine::InsertPosIndication::At:
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "At指定は行えません。(id=", id, ",pos_id=", pos_id, ")");
				return false;
				break;
			case planeta_engine::InsertPosIndication::Before:
				id_it_map_.emplace(std::make_pair(id, key_position_id_list_.insert(list_it, id)));
				break;
			case planeta_engine::InsertPosIndication::After:
				++list_it;
				id_it_map_.emplace(std::make_pair(id, key_position_id_list_.insert(list_it, id)));
				break;
			default:
				break;
			}
			return true;
		}
	}
}