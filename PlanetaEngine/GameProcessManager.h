#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "SceneModule.h"
#include "GameProcessManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class IGameAccessor;
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForGameProcess;
	}
	namespace game {
		class GameProcess;
		class GameProcessManager final: public core::SceneModule
			,public GameProcessManagerPublicInterface{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager() = default;
			/*����������*/
			bool Initialize()override { return true; }
			/*�I������*/
			void Finalize()override;
			/*�V�[���C���^�[�t�F�C�X���Z�b�g*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*�V�[���f�[�^���Z�b�g*/
			void SetSceneData(const core::SceneData& scene_data)override;

			/*�v���Z�X�̃A�b�v�f�[�g*/
			void Update();
			/*�Ǘ�����*/
			bool Process();
			/*ID�Ńv���Z�X�擾*/
			utility::WeakPointer<GameProcess> GetGameProcess(int id) override;
			/*ID�Ńv���Z�X�폜*/
			bool RemoveGameProcess(int id)override;


			/*���O�ŃV�X�e���v���Z�X�擾*/
			utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name)override {
				auto it = _system_process_map.find(name);
				return it == _system_process_map.end() ? nullptr : *it->second;
			}
			/*�V�X�e���v���Z�X�ǉ�
			�폜�s�B�ŗL��������*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority, const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_, -1);
				SetupProcess(new_proc);
				_game_processes[priority].emplace_back(new_proc);
				_system_process_map.emplace(name, --_game_processes[priority].end());
				return new_proc;
			}
		private:
			/*�Q�[���v���Z�X���X�g
			�D��x�̒Ⴂ���ɕ��ׂ邽�߂�map���g�p�B�v�f�̒ǉ��폜�ŃC�e���[�^���������ɂȂ�Ȃ����Ƃ���list���g�p*/
			std::map<int, std::list<std::shared_ptr<GameProcess>>> _game_processes;
			/*ID�}�b�v�B<id,<�D��x�A�v���Z�X���X�g�̃C�e���[�^>>*/
			std::unordered_map<int, std::pair<int, std::list<std::shared_ptr<GameProcess>>::iterator>> _id_map;

			/**/
			int _id_counter;
			/*�o�^����ID���X�g*/
			std::vector<int> _unresist_id_list;
			/*����v���Z�X�}�b�v�B<�v���Z�X��,�v���Z�X���X�g�̃C�e���[�^>*/
			std::unordered_map<std::string, std::list<std::shared_ptr<GameProcess>>::iterator> _system_process_map;
			/*�`�F�b�J�[��ʉ߂���V�X�e���v���Z�X���擾*/
			utility::WeakPointer<GameProcess> GetSystemProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const {
				for (const auto& sp : _system_process_map) {
					if (type_checker(*sp.second)) { return *sp.second; }
				}
				return nullptr;
			}
			/*�v���Z�X�ǉ�(ID��Ԃ��B-1�ŃG���[)*/
			int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process);
			/*�Q�[���v���Z�X�쐻*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority)override {
				auto np = creator(game_, _id_counter++);
				return AddGameProcess(priority, np) >= 0 ? np : nullptr;
			}
			/*�V�X�e���v���Z�X�ǉ�*/
			bool CreateSystemProcess(const std::shared_ptr<GameProcess>& game_process, int priority) { return AddGameProcess(priority, game_process) >= 0; }
			/*�v���Z�X�̐ݒ�*/
			void SetupProcess(const std::shared_ptr<GameProcess>& game_process);

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
		};
	}
}