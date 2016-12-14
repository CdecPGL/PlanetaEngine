#pragma once

#include "GameObjectSystemComponent.h"
#include "Vector2D.h"

namespace planeta {
	class CTransformGUI final : public private_::GameObjectSystemComponent{
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CTransformGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CTransformGUI();
		~CTransformGUI();
		/*�A�N�Z�T*/
		//! �ʒu���擾
		const Vector2Di& position()const;
		//! �ʒu��ݒ�
		CTransformGUI& position(const Vector2Di& pos);
		//! ��]�x���擾
		const double rotation_rad()const;
		//! ��]�x��ݒ�
		CTransformGUI&  rotation_rad(double rota_rad);
		//! �T�C�Y��ݒ�
		CTransformGUI& size(const Vector2Di& s);
		//! �T�C�Y���擾
		const Vector2Di& size()const;
		//! ���S�ʒu��ݒ�(0.0-1.0)
		CTransformGUI& pivot(const Vector2Dd& p);
		//! ���S�ʒu���擾(0.0-1.0)
		const Vector2Dd& pivot()const;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		//GOC�I�[�o�[���C�h
		bool OnActivated()override final;
		bool OnInactivated()override final;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CTransformGUI);
}