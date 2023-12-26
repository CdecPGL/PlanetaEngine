#pragma once

#include <string>
#include "Vector2D.hpp"
#include "InputDefinitions.hpp"

namespace plnt {
	/*! �C���v�b�g�}�l�[�W���̌��J�C���^�[�t�F�C�X*/
	class IInputManager {
	public:
		virtual ~IInputManager() = 0 { };
		/*@brief �L�[�̓��͏�Ԃ��擾(�����w��s��)
		@param �L�[�R�[�h*/
		virtual bool KeyState(Key::type) const = 0;
		/*@brief �p�b�h�̓��͏�Ԃ��擾(�����w��s��)
		@param �p�b�h�R�[�h*/
		virtual bool PadState(Pad::type) const = 0;
		/*@brief �{�^���̓��͏�Ԃ��擾(�����w��s��)
		@param �{�^���R�[�h*/
		virtual bool ButtonState(Button::type) const = 0;
		/*@brief �{�^���̓��͏�Ԃ��擾(�����w��s��)
		@param �{�^����*/
		virtual bool ButtonState(const std::string &) const = 0;
		/*@brief �}�E�X�{�^���̓��͏�Ԃ��擾(�����w��s��)
		@param �}�E�X�{�^���R�[�h*/
		virtual bool MouseButtonState(MouseButton::type) const = 0;
		/*@brief �L�[�̉����擾(�����w��s��)
		@param �L�[�R�[�h*/
		virtual bool KeyPush(Key::type) const = 0;
		/*@brief �p�b�h�̉����擾(�����w��s��)
		@param �p�b�h�R�[�h*/
		virtual bool PadPush(Pad::type) const = 0;
		/*@brief �{�^���̉����擾(�����w��s��)
		@param �{�^���R�[�h*/
		virtual bool ButtonPush(Button::type) const = 0;
		/*@brief �{�^���̉����擾(�����w��s��)
		@param �{�^����*/
		virtual bool ButtonPush(const std::string &) const = 0;
		/*@brief �}�E�X�{�^���̉����擾(�����w��s��)
		@param �}�E�X�{�^���R�[�h*/
		virtual bool MouseButtonPush(MouseButton::type) const = 0;
		/*! �}�E�X�|�C���^�̈ʒu���擾*/
		virtual const Vector2Di GetMousePointerPosition() const = 0;
		/*! �}�E�X�z�C�[���̉�]�ʂ��擾*/
		virtual int GetMouseWheelRotation() const = 0;
	};
}
