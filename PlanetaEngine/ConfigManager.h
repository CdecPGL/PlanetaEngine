#pragma once

#include "IConfigManager.h"
#include "SubSystemManager.h"

namespace planeta {
	class File;
	namespace private_ {
		class ConfigManager : public IConfigManager, public SubSystemManager {
		public:
			virtual ~ConfigManager()override = default;
			virtual bool LoadSystemConfig(const File& file) = 0;
			virtual bool LoadUserConfig(const File& file) = 0;
			/*! �J���[�r�b�g�[�x���擾����*/
			virtual int color_bit_depth()const = 0;
			/*! �X�^�[�g�A�b�v�V�[��ID���擾����*/
			virtual std::string startup_scene_id()const = 0;
		};
	}
}
