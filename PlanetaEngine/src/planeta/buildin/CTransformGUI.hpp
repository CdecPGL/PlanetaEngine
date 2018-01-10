#pragma once

#include "planeta/GameObjectSystemComponent.hpp"
#include "planeta/RectAngle.hpp"

namespace plnt {
	//! GUI�p�g�����X�t�H�[���R���|�[�l���g(�e�q�֌W�֘A�̋@�\�͖�����)
	class CTransformGUI final : public private_::GameObjectSystemComponent{
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransformGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CTransformGUI();
		~CTransformGUI();
		/*�A�N�Z�T*/
		//! �ʒu���擾�BPivot����_�Ƃ���
		const Vector2Di& position()const;
		//! �ʒu��ݒ�BPivot����_�Ƃ���
		CTransformGUI& position(const Vector2Di& pos);
		//! ��]�x���擾�BPivot�𒆐S�Ƃ���
		const double rotation_rad()const;
		//! ��]�x��ݒ�BPivot�𒆐S�Ƃ���
		CTransformGUI& rotation_rad(double rota_rad);
		//! �T�C�Y���擾�BPivot����_�Ƃ���
		const Vector2Di& size()const;
		//! �T�C�Y��ݒ�BPivot����_�Ƃ���
		CTransformGUI& size(const Vector2Di& s);
		//! �ό`���S�ʒu���擾(0.0-1.0)
		const Vector2Dd& pivot()const;
		//! �ό`���S�ʒu��ݒ�(0.0-1.0)
		CTransformGUI& pivot(const Vector2Dd& p);
		//! GUI�̈��\����`���擾�B��]�͍l������Ȃ�
		//const RectAnglei& rect()const;
		//! �A���J�[����̈ʒu���擾�B�eCTransformGUI���ł̈ʒu��(0.0-1.0)^2�ŕ\��
		//Vector2Dd anchor_min()const;
		//! �A���J�[����̈ʒu��ݒ�B�eCTransformGUI���ł̈ʒu��(0.0-1.0)^2�ŕ\��
		//CTransformGUI& anchor_min(const Vector2Dd& a);
		//! �A���J�[�E���̈ʒu���擾�B�eCTransformGUI���ł̈ʒu��(0.0-1.0)^2�ŕ\��
		//Vector2Dd anchor_max()const;
		//! �A���J�[�E���̈ʒu��ݒ�B�eCTransformGUI���ł̈ʒu��(0.0-1.0)^2�ŕ\��
		//CTransformGUI& anchor_max(const Vector2Dd& a);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOC�I�[�o�[���C�h
		void OnActivated()override final;
		void OnInactivated()override final;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CTransformGUI);
}