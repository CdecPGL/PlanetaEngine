#pragma once
#include <memory>
#include <functional>

#include"Object.h"
#include "NonCopyable.h"
#include "BadNewDeleteOperation.h"
#include "SharedPointerInstance.h"
#include "WeakPointer.h"
#include "NonOwingPointer.h"
#include "GOComponentGetter.h"

namespace planeta {
	class SceneAccessorForGameObject;
	class JSONObject;
	namespace core{
		struct SceneData;
		struct GameObjectComponentSetUpData;
	}
	class IGameObjectForComponent;
	/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g�̊��N���X
		@warning �Q�[���I�u�W�F�N�g�R���|�[�l���g�̒�`�̍ۂɂ́A���̃N���X�𒼐ڌp�����Ă͂����Ȃ��B�����GameObjectStandardComponent���p�����邱�ƁB
	*/
	class GameObjectComponent : public core::Object, public std::enable_shared_from_this<GameObjectComponent>, private util::NonCopyable<GameObjectComponent>{
	public:
		GameObjectComponent() = default;
		virtual ~GameObjectComponent() = default;
		bool SystemSetUp(const core::GameObjectComponentSetUpData& resistration_data);

		/*! @brief �Q�[���I�u�W�F�N�g�R���|�[�l���g���g�p�\��
			
			���L����Ă���Q�[���I�u�W�F�N�g���j������Ă���Ƃ���A����������������Ă��Ȃ��Ƃ���false��Ԃ��B
		*/
		bool is_valied()const { return is_valied_; }
		//! ���L����Ă���Q�[���I�u�W�F�N�g���V�[�����ŗL����Ԃ�
		bool is_active()const { return is_active_; }

		/*�V�X�e���֐�(GameObjectBase����Ăяo�����)*/
		//�t�@�C���̓ǂݍ���
		bool Load(const JSONObject& json_obj);
		//�ق��R���|�[�l���g�擾�Ə�����
		bool Initialize(const GOComponentGetter& com_getter);
		bool Activate();
		bool InActivate();
		void Finalize();
	protected:
		/*! @brief ���L����Ă���Q�[���I�u�W�F�N�g�ւ̎Q��
			
			�ʏ��IGameObjectInterface�ł͂Ȃ��A�Q�[���I�u�W�F�N�g�R���|�[�l���g�p�Ɋ֐����ǉ����ꂽ���̂�Ԃ��B
		*/
		IGameObjectForComponent& game_object();
		//�I�[�o�[���C�h�\�֐�
		//! �R���|�[�l���g�擾�����֐�
		virtual bool GetOtherComponentProc(const GOComponentGetter& com_getter) { return true; }
		/*! @brief ���[�h�����֐�
			
			���L����Ă���Q�[���I�u�W�F�N�g�̏������O�ɌĂяo�����B
			������GOComponentGetter��p���ď��L����Ă���Q�[���I�u�W�F�N�g�̑��̃R���|�[�l���g���擾����B<br/>
			�f�t�H���g�ł�false��Ԃ��̂ŁA�t�@�C����`�ǂݍ��݂ɑΉ����������ꍇ�͕K���I�[�o�[���C�h����B<br/>
			���̊֐���false��Ԃ��R���|�[�l���g���܂ރQ�[���I�u�W�F�N�g���t�@�C����`����ǂݍ��ޏꍇ�A��`�ɂ��̃R���|�[�l���g�̒�`���܂܂�Ă���ƃG���[�ɂȂ�B<br/>
			�擪�Őe�N���X�̓��֐����Ăяo���K�v������B

			@attention ���݂́A���L����Ă���Q�[���I�u�W�F�N�g���Ƃ̏������O�ɌĂяo����邪�A����A�f�[�^�̃��[�h���Ɉ�x�����Ăяo�����悤�ɕύX�����\������B
			@param �f�[�^���i�[����JSONObject
			@return �f�[�^�ǂݍ��݂̐���
		*/
		virtual bool LoadProc(const JSONObject& json_obj) { return false; }
		/*�C�x���g�֐�*/
		/*! @brief ���������C�x���g�֐�

			��������Q�[���I�u�W�F�N�g�����������ꂽ�Ƃ��ɌĂяo�����B<br/>
			�擪�Őe�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnInitialized() { return true; };
		/*! @brief �L�������C�x���g�֐�

			��������Q�[���I�u�W�F�N�g���L�������ꂽ�Ƃ��ɌĂяo�����B<br/>
			�擪�Őe�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnActivated() { return true; };
		/*! @brief ���������C�x���g�֐�

		��������Q�[���I�u�W�F�N�g�����������ꂽ�Ƃ��ɌĂяo�����B<br/>
		�����Őe�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual bool OnInactivated() { return true; } //��������Q�[���I�u�W�F�N�g���V�[������o�^���������Ƃ��ɌĂяo����(�V�X�e���֐�)
		/*! @brief �j�����C�x���g�֐�

			��������Q�[���I�u�W�F�N�g���j������钼�O�ɌĂяo�����B<br/>
			�����Őe�N���X�̓��֐����Ăяo���K�v������B
		*/
		virtual void OnFinalized()noexcept {} //��������Q�[���I�u�W�F�N�g���j�������Ƃ��ɌĂяo�����(�V�X�e���֐�)
	private:
		bool is_valied_ = false;
		bool is_active_ = false;

		util::NonOwingPointer<IGameObjectForComponent> game_object_;

		/*���ʐݒ�֐�*/
		virtual void SetSceneData(const util::WeakPointer<core::SceneData>& scene_data) = 0;
	};
}

/*! @def
	GameObejectComponent���V�X�e���ɓo�^����
	@param(type) �o�^�������^
*/
#define PE_REGISTER_GAMEOBJECTCOMPONENT(type)\
PE_REGISTER_OBJECT(type,planeta::GameObjectComponent)