#pragma once

#include "Object.h"
#include <string>
#include <unordered_map>
#include<memory>
#include <vector>
#include"KeyInputDefines.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace core {
		class KeyInputManager final {
		public:
			KeyInputManager();
			~KeyInputManager();
			/*������*/
			bool Initialize();
			/*�X�V*/
			bool Update();
			/*���ׂĂ̐ݒ�����Z�b�g����*/
			void ResetAllSettings();
			/*�L�[�ƃp�b�h�̃{�^�����蓖�Ă����Z�b�g����*/
			void ResetKeyPadButtonMap();
			/*�{�^���̕ʖ������Z�b�g����*/
			void ResetButtonAlias();
			/*�f�t�H���g�̐ݒ�l�ɖ߂�*/
			void SetDefaultSettings();
			/*@brief �L�[�̓��͏�Ԃ��擾(�����w��s��)
			@param �L�[�R�[�h�A�^�O*/
			bool KeyState(Key::type, const std::string& = "\0")const;
			/*@brief �p�b�h�̓��͏�Ԃ��擾(�����w��s��)
			@param �p�b�h�R�[�h�A�^�O*/
			bool PadState(Pad::type, const std::string& = "\0")const;
			/*@brief �{�^���̓��͏�Ԃ��擾(�����w��s��)
			@param �{�^���R�[�h�A�^�O*/
			bool ButtonState(Button::type, const std::string& = "\0")const;
			/*@brief �{�^���̓��͏�Ԃ��擾(�����w��s��)
			@param �{�^�����A�^�O*/
			bool ButtonState(const std::string&, const std::string& = "\0")const;
			/*@brief �}�E�X�{�^���̓��͏�Ԃ��擾(�����w��s��)
			@param �}�E�X�{�^���R�[�h�A�^�O*/
			bool MouseButtonState(MouseButton::type, const std::string& = "\0")const;
			/*@brief �L�[�̉����擾(�����w��s��)
			@param �L�[�R�[�h�A�^�O*/
			bool KeyPush(Key::type, const std::string& = "\0")const;
			/*@brief �p�b�h�̉����擾(�����w��s��)
			@param �p�b�h�R�[�h�A�^�O*/
			bool PadPush(Pad::type, const std::string& = "\0")const;
			/*@brief �{�^���̉����擾(�����w��s��)
			@param �{�^���R�[�h�A�^�O*/
			bool ButtonPush(Button::type, const std::string& = "\0")const;
			/*@brief �{�^���̉����擾(�����w��s��)
			@param �{�^�����A�^�O*/
			bool ButtonPush(const std::string&, const std::string& = "\0")const;
			/*@brief �}�E�X�{�^���̉����擾(�����w��s��)
			@param �}�E�X�{�^���R�[�h�A�^�O*/
			bool MouseButtonPush(MouseButton::type, const std::string& = "\0")const;
			/*�{�^���ɃL�[�����蓖�Ă�(�L�[�͕����w���)*/
			void AssignKeyToButton(Key::type, Button::type);
			/*�{�^���Ƀp�b�h���͂����蓖�Ă�(�p�b�h�͕����w���)*/
			void AssignPadToButton(Pad::type, Button::type);
			/*�{�^���ɕʖ�������B�����̏ꍇ�͊��蓖�ĂȂ���(�����w��s��)*/
			void AssignAliasToButton(const std::string&, Button::type);
			/*�{�^���Ɋ��蓖�Ă�ꂽ�L�[�}�b�v�擾*/
			const std::unordered_map<Button::type, std::vector<Key::type>>& GetAssignedKeyToButtonMap()const;
			/*�{�^���Ɋ��蓖�Ă�ꂽ�L�[�}�b�v�擾*/
			const std::unordered_map<Button::type, std::vector<Pad::type>>& GetAssignedPadToButtonMap()const;
			/*�w��^�O�̓��͂𖳌���*/
			void InvalidateInputByTag(const std::string&);
			/*�w��^�O�̓��͂�L����*/
			void ValidateInputByTag(const std::string&);
			/*�w��^�O�̓��͂��������m�F����*/
			bool IsInputInvalidByTag(const std::string&)const;
			/*�^�O�̓��͖����������Z�b�g����*/
			void ResetInvalidTag();
			/*�}�E�X�|�C���^�̈ʒu���擾*/
			const Vector2Di GetMousePointerPosition()const;
			/*�}�E�X�z�C�[���̉�]�ʂ��擾*/
			int GetMouseWheelRotation()const;
		private:
			KeyInputManager(const KeyInputManager&) = delete;
			KeyInputManager(KeyInputManager&&) = delete;
			const KeyInputManager& operator = (const KeyInputManager&) = delete;
			const KeyInputManager& operator = (KeyInputManager&&) = delete;
			class Impl;
			std::unique_ptr<Impl> _impl;
		};
	}
}
