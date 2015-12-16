#pragma once
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "SceneModule.h"
#include "GameProcessManagerPublicInterface.h"
#include "WeakPointer.h"
#include "InsertPosIndication.h"
#include "GameProcessListTypes.h"

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
			�폜�s��*/
			template<class C>
			utility::WeakPointer<C> AddSystemProcess(const core::GameProcessPositionInList& pos,InsertPosIndication ins_pos = InsertPosIndication::At) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(game_);
				SetupProcess_(new_proc, []() {return true; });
				auto pos = AddGameProcessToList_(priority, new_proc);
				return new_proc;
			}
		private:
			/*�Q�[���v���Z�X���X�g
			�v�f�̒ǉ��폜�ŃC�e���[�^�������ɂȂ�Ȃ����Ƃ���list���g�p*/
			core::GameProcessPriorytyListType process_priority_list_;
			/*ID�}�b�v�B<id,���X�g���ʒu>*/
			using ProcessIDMapType = std::unordered_map<GameProcess*, core::GameProcessPositionTypeAtList>;
			ProcessIDMapType process_id_map_;
			/*���OID�}�b�v�B<�v���Z�X��,ID>*/
			using ProcessNameMapType = std::unordered_map<std::string, GameProcess*>;
			ProcessNameMapType process_name_id_map_;
			/*�o�^�������X�g*/
			std::vector<core::GameProcessPositionTypeAtList> dispose_list_;
			/*�L�[�|�W�V�����}�b�v*/
			std::unordered_map<std::string, core::GameProcessPriorytyListType> key_position_map_;

			/*���O����Q�[���v���Z�X���擾*/
			utility::WeakPointer<GameProcess> GetProcess(const std::string& name)override;
			/*�Q�[���v���Z�X�쐻*/
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication)override;
			std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication, const std::string& name)override;
			/*�L�[�|�W�V�����̐ݒ�*/
			bool SetKeyPosition(const core::GameProcessPositionInList& pos, const std::string& id)override;

			/*�v���Z�X�̐ݒ�*/
			void SetupProcess_(const std::shared_ptr<GameProcess>& game_process, std::function<bool()>&& remover,const core::GameProcessPositionInList& pos);
			/*�v���Z�X�����X�g�ɒǉ�*/
			core::GameProcessPositionTypeAtList AddGameProcessToList_(const std::shared_ptr<GameProcess>& game_process, const core::GameProcessPositionInList& pos, InsertPosIndication ins_pos);
			/*���O��o�^*/
			bool RegisterProcessName_(const std::string& name,GameProcess* id);
			/*ID�}�b�v�Ƀv���Z�X�o�^*/
			bool RegisterToIDMap_(GameProcess* id,const core::GameProcessPositionTypeAtList& pos_at_plist);
			/*�ʒu�Ńv���Z�X�폜*/
			bool RemoveGameProcess_(const core::GameProcessPositionInList& pos);
			/*�j�����X�g����������*/
			void ProcessDisposeList_();

			core::IGameAccessor& game_;
			std::shared_ptr<core::SceneAccessorForGameProcess> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameProcess> scene_data_;
		};
	}
}