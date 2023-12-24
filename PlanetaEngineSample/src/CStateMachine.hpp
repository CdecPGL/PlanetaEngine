#pragma once

#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <list>

#include "boost/optional.hpp"

#include "planeta/PlanetaEngine.hpp"


/*! @brief ��ԑJ�ڃ}�V�[���R���|�[�l���g

	GameObject�̏������֐������R���|�[�l���g�ǉ��֐��ŁA�X�e�[�g�̐ݒ���s���B
	�J�ڎ��Ɏq�X�e�[�g�܂Ŏw�肵���ꍇ�́A�r����Enetr�֐��̖߂�l�͖��������B

	@todo �G���[���b�Z�[�W�ŃX�e�[�g�p�X���o�͂���悤�ɂ���B�X�e�[�g�p�X�ɂ��}�b�v��ێ����Ă����āA�p�X�ɂ�錟��������������B
*/

namespace plnt {
	class IGameObject;
}
class CStateMachine : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CStateMachine);
private:
	class StateBase {
	public:
		virtual ~StateBase() = default;
		virtual boost::optional<std::string> EnterProcBase() = 0;
		virtual boost::optional<std::string> UpdateProcBase() = 0;
		virtual void ExitProcBase() = 0;
		virtual boost::optional<std::string> MessageHandlerBase(const std::string&) = 0;
	};
public:
	using Super = plnt::GameObjectStandardComponent;
	CStateMachine();
	template<class S>
	bool RegisterState(const std::string& state_path, typename S::ArgType& arg_com);
	/*! �X�e�[�g�̊�{�ƂȂ�N���X�B�e���v���[�g�����ɂ̓X�e�[�g�֐��̈����ƂȂ�R���|�[�l���g�^���w�肷��*/
	template<class C>
	class State : public StateBase {
		template<class S>
		friend bool CStateMachine::RegisterState(const std::string&, typename S::ArgType&);
	public:
		using ArgType = C;
		virtual ~State() = default;
		/*! @brief �X�e�[�g�̊J�n���ɌĂ΂��֐��B
			
			�߂�l�őJ�ڐ�̃X�e�[�g��e����Ƃ��鑊�΃p�X�Ŏw��\*/
		virtual boost::optional<std::string> EnterProc(C&) { return boost::none; };
		/*! @brief �X�e�[�g�̍X�V���ɌĂ΂��֐��B

		�߂�l�őJ�ڐ�̃X�e�[�g��e����Ƃ��鑊�΃p�X�Ŏw��\*/
		virtual boost::optional<std::string> UpdateProc(C&) { return boost::none; };
		/*! @brief �X�e�[�g�̏I�����ɌĂ΂��֐��B*/
		virtual void ExitProc(C&) {};
		/*! @brief ���b�Z�[�W���󂯎�����Ƃ��ɌĂ΂��֐��B

		�߂�l�őJ�ڐ�̃X�e�[�g��e����Ƃ��鑊�΃p�X�Ŏw��\*/
		virtual boost::optional<std::string> MessageHandler(C&, const std::string&) { return boost::none; };
	private:
		C* arg_com_ = nullptr;
		void SetArgCom_(C& arg_com) {
			arg_com_ = &arg_com;
		}

		boost::optional<std::string> EnterProcBase()final {
			return EnterProc(*arg_com_);
		}
		boost::optional<std::string> UpdateProcBase()final {
			return UpdateProc(*arg_com_);
		}
		void ExitProcBase()final {
			ExitProc(*arg_com_);
		}
		boost::optional<std::string> MessageHandlerBase(const std::string& mess)final {
			return MessageHandler(*arg_com_, mess);
		}
	};
	//!�J��(.��؂�ŁA�q�X�e�[�g�ɂ��A�N�Z�X�\)
	bool Transition(const std::string& state_path);
	//!���b�Z�[�W�𑗐M
	bool SendMessage(const std::string& message);
	//!�X�e�[�g�}�V�����J�n(.��؂�ŁA�q�X�e�[�g�ɂ��A�N�Z�X�\)
	bool StartStateMachine(const std::string& state_path);
	//!�X�e�[�g�}�V�����~
	void StopStateMachine();
private:
	void OnInitialized()override;

	struct StateData;
	//�X�e�[�g�K�w�\�����ł̃C���f�b�N�X-1���C���f�b�N�X�Ƃ������s���X�e�[�g�̃��X�g�B���[�g�X�e�[�g�͊܂܂�Ȃ��B
	std::vector<std::pair<size_t, std::shared_ptr<StateBase>>> active_state_list_;
	//�o�^���ꂽ�X�e�[�g�̊Ǘ��p�f�[�^
	struct StateData {
		std::string state_id; //�X�e�[�gID
		std::function<std::shared_ptr<StateBase>()> creator; //�X�e�[�g�N���G�[�^
		size_t parent_state_data_idx; //�e�X�e�[�g�̃C���f�b�N�X
		std::unordered_map<std::string, size_t> child_state_data_idxes; //�q�X�e�[�g�̃C���f�b�N�X
		size_t state_hierarchy_depth; //���[�g�X�e�[�g��0�Ƃ����K�w�\�����ł̐[�x
	};
	//�X�e�[�gID�ƃC���f�b�N�X�̃}�b�v(�R�����g��������Ȃ�A�L�[���X�e�[�g�p�X�Ŏ��ׂ��B���̂��߂ɁA�X�e�[�g�f�[�^�͎̂ĂO�ƃp�X���ێ����Ă����ׂ�)
	//std::unordered_map<std::string, size_t> state_data_map_;
	//�X�e�[�g�f�[�^���X�g�B�C���f�b�N�X0�̓��[�g�X�e�[�g
	std::vector<StateData> state_data_list_;

	bool RegisterState_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_path);
	bool RegisterStateWithParentStateDataIdx_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_id, size_t parent_state_data_idx);
	 //�����̓X�e�[�g�p�X
	bool Transition_(const std::string& state_path);
	bool ParentBaseTransition_(const StateData& parent_state_data, std::list<std::string>& ids);
	bool MeBaseTransition_(const StateData& state_data, const std::string& relative_state_path);
	void Update_();
};
PE_GAMEOBJECTCOMPONENT_CLASS(CStateMachine);

//! �X�e�[�g��o�^(�����ƂȂ�Q�[���I�u�W�F�N�g�R���|�[�l���g�͂��̃R���|�[�l���g�Ɠ���GameObject�ɕێ�����Ă��Ȃ���΂Ȃ�Ȃ��B)
template<class S>
bool CStateMachine::RegisterState(const std::string& state_path, typename S::ArgType& arg_com) {
	//�e���v���[�g�p�����[�^��StateBase���p�����Ă��邩�m�F
	static_assert(std::is_base_of_v<StateBase, S>, "The template paremeter S must derive StateBase");
	//�o�^���ꂽGOC��GameObject�Ǝ����̃Q�[���I�u�W�F�N�g���������̂��w���Ă��邩�m�F
	PE_VERIFY(&game_object() == &arg_com.game_object());
	//���̃R���|�[�l���g�̎�����arg_com�̎Q�Ɛ�̎����͓����ł���(����GameObject�̃R���|�[�l���g���)���Ƃ�O��Ƃ���
	return RegisterState_([&arg_com] {auto s = std::make_shared<S>(); s->SetArgCom_(arg_com); return s; }, state_path);
}
