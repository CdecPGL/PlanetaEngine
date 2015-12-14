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
		struct SceneDataForGameProcess;
		class SceneAccessorForGameProcess;
	}
	namespace game {
		class GameProcess;
		class GameProcessManager final: public core::SceneModule
			,public GameProcessManagerPublicInterface{
		public:
			GameProcessManager(core::IGameAccessor& game);
			~GameProcessManager();
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

			/*�V�X�e���v���Z�X�ǉ�(�V�X�e���v���Z�X��ID�}�b�v�▼�O�}�b�v�ɓo�^���Ȃ�)
			�폜�s�B�ŗL��������*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				SetupProcess_(new_proc, []() {return true; });
				auto pos = AddGameProcessToList_(priority, new_proc);
				return new_proc;
			}
		private:
			/*�Q�[���v���Z�X�D��x���X�g
			�D��x�̒Ⴂ���ɕ��ׂ邽�߂�map���g�p�B�v�f�̒ǉ��폜�ŃC�e���[�^���������ɂȂ�Ȃ����Ƃ���list���g�p*/
			using ProcessListType =std::list<std::shared_ptr<GameProcess>>;
			using ProcessPriorytyListType = std::map<int, ProcessListType>;
			ProcessPriorytyListType process_priority_list_;
			using PositionTypeAtPriorytyList = std::pair<ProcessPriorytyListType::iterator, ProcessListType::iterator>;
			/*ID�}�b�v�B<id,<�D��x���X�g�C�e���[�^�A�v���Z�X���X�g�C�e���[�^>>*/
			using ProcessIDMapType = std::unordered_map<GameProcess*, PositionTypeAtPriorytyList>;
			ProcessIDMapType process_id_map_;
			/*���OID�}�b�v�B<�v���Z�X��,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, GameProcess*>;
			ProcessNameMapType process_name_id_map_;
			/*�o�^�������X�g*/
			std::vector<PositionTypeAtPriorytyList> dispose_list_;
			/*�`�F�b�J�[��ʉ߂���ŏ��̃v���Z�X���擾*/
			utility::WeakPointer<GameProcess> GetProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const {
				for (const auto& sp : process_id_map_) {
					if (type_checker(*sp.second.second)) { return *sp.second.second; }
				}
				return nullptr;
			}
			/*���O����Q�[���v���Z�X���擾*/
			utility::WeakPointer<GameProcess> GetProcess(const std::string& name)override;
			/*�Q�[���v���Z�X�쐻*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority)override {
				auto np = creator(game_);
				SetupProcess_(np, [id = np.get(), this](){return RemoveGameProcess_(id); });
				auto pos = AddGameProcessToList_(priority, np);
				return RegisterToIDMap_(np.get(), pos) ? np : nullptr;
			}
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority,const std::string& name)override {
				auto np = creator(game_);
				SetupProcess_(np, [id = np.get(), this](){return RemoveGameProcess_(id); });
				auto pos = AddGameProcessToList_(priority, np);
				return (RegisterToIDMap_(np.get(), pos) && RegisterProcessName_(name, np.get())) ? np : nullptr;
			}

			/*�v���Z�X�̐ݒ�*/
			void SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover);
			/*�v���Z�X�����X�g�ɒǉ�*/
			PositionTypeAtPriorytyList AddGameProcessToList_(int priority, const std::shared_ptr<GameProcess>& game_process);
			/*���O��o�^*/
			bool RegisterProcessName_(const std::string& name,GameProcess* id);
			/*ID�}�b�v�Ƀv���Z�X�o�^*/
			bool RegisterToIDMap_(GameProcess* id,const PositionTypeAtPriorytyList& pos_at_plist);
			/*ID�Ńv���Z�X�폜*/
			bool RemoveGameProcess_(GameProcess* id);
			/*�j�����X�g����������*/
			void ProcessDisposeList_();

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameProcess> scene_data_;
		};
	}
}