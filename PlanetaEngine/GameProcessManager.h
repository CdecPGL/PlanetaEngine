#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "Object.h"
#include "IGameProcessManagerAccessor.h"
#include "IGameProcessManagerSetup.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class IGameAccessor;
		class ISceneAccessForGameProcess;
	}
	namespace game {
		class IUIManagerAccessor;
		class IGameObjectManagerAccessor;
		class GameProcess;
		class GameProcessManager final: public core::Object
			,public IGameProcessManagerAccessor,public IGameProcessManagerSetup{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager() = default;
			/*�e��}�l�[�W���|�C���^�[�̃Z�b�g*/
			void SetManagerPointer(const utility::WeakPointer<core::ISceneAccessForGameProcess>& scene) { _scene = scene; }
			/*�I������*/
			bool Finalize();
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
			/*�^�ŃV�X�e���v���Z�X�擾*/
			/*template<class C>
			std::shared_ptr<C> GetSystemProcess() {
				for (const auto& p : _system_process_map) {
					std::shared_ptr<C> ptr = std::dynamic_pointer_cast<C>(*p.second);
					if (ptr) { return ptr; }
				}
				return nullptr;
			}*/
			/*�V�X�e���v���Z�X�ǉ�
			�폜�s�B�ŗL��������*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority, const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(_game, -1);
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
			std::unordered_map<std::string, std::list<std::shared_ptr<GameProcess>>::iterator>& _GetSystemProcessList()override { return _system_process_map; }
			/*�v���Z�X�ǉ�(ID��Ԃ��B-1�ŃG���[)*/
			int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process)override;
			/*��������ID���擾����*/
			int GetNewID()override { return _id_counter++; }
			core::IGameAccessor& GetGameAccessorReference()override { return _game; }
			/*�v���Z�X�̐ݒ�*/
			void SetupProcess(const std::shared_ptr<GameProcess>& game_process);

			core::IGameAccessor& _game;
			utility::WeakPointer<core::ISceneAccessForGameProcess> _scene;
		};
	}
}