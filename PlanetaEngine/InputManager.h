#pragma once

#include <unordered_map>
#include <vector>
#include "IInputManager.h"
#include "SubSystemManager.h"

namespace planeta {
	/*�C���v�b�g�}�l�[�W��*/
	namespace private_ {
		class InputManager : public IInputManager, public SubSystemManager {
		public:
			virtual ~InputManager()override = default;
			virtual bool Initialize() = 0;
			virtual void Finalize() = 0;
			virtual bool Update() = 0;
			/*���ׂĂ̐ݒ�����Z�b�g����*/
			virtual void ResetAllSettings() = 0;
			/*�L�[�ƃp�b�h�̃{�^�����蓖�Ă����Z�b�g����*/
			virtual void ResetKeyPadButtonMap() = 0;
			/*�{�^���̕ʖ������Z�b�g����*/
			virtual void ResetButtonAlias() = 0;
			/*�f�t�H���g�̐ݒ�l�ɖ߂�*/
			virtual void SetDefaultSettings() = 0;
			/*�{�^���ɃL�[�����蓖�Ă�(�L�[�͕����w���)*/
			virtual void AssignKeyToButton(Key::type, Button::type) = 0;
			/*�{�^���Ƀp�b�h���͂����蓖�Ă�(�p�b�h�͕����w���)*/
			virtual void AssignPadToButton(Pad::type, Button::type) = 0;
			/*�{�^���ɕʖ�������B�����̏ꍇ�͊��蓖�ĂȂ���(�����w��s��)*/
			virtual void AssignAliasToButton(const std::string&, Button::type) = 0;
			/*�{�^���Ɋ��蓖�Ă�ꂽ�L�[�}�b�v�擾*/
			virtual const std::unordered_map<Button::type, std::vector<Key::type>>& GetAssignedKeyToButtonMap()const = 0;
			/*�{�^���Ɋ��蓖�Ă�ꂽ�L�[�}�b�v�擾*/
			virtual const std::unordered_map<Button::type, std::vector<Pad::type>>& GetAssignedPadToButtonMap()const = 0;
		};
	}
}
