#pragma once

#include "planeta/PlanetaEngine.hpp"

#include "EADamage.hpp"

/*! �_���[�W��^����R���|�[�l���g

CDamageReceiver�ƃZ�b�g�Ŏg���B"DamageReceiver"���x�����ݒ肳��Ă���R���C�_�[��"DamageAdder"���x�����ݒ肳��Ă���R���C�_�[���Փ˂���ƁADamageReceiver�̕���CdamageReceiver::AddDamage���Ăяo���B
*/
class CDamageAdder final: public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDamageAdder);
public:
	using Super = plnt::GameObjectStandardComponent;
	/*! �U���͂��擾����*/
	int power()const;
	/*! �U���͂�ݒ肷��*/
	void power(int p);
	/*! �^���閳�G�t���[�����Ԃ��擾����*/
	int add_invincible_frame()const;
	/*! �^���閳�G�t���[�����Ԃ�ݒ肷��*/
	void add_invincible_frame(int inv_f);
	/*! �_���[�W��^�����Ƃ��̃C�x���g*/
	plnt::Signal<void(const EADamageAdded&)> damage_added;
private:
	int add_invincible_frame_ = 1;
	int power_ = 1;
	bool GetOtherComponentsProc(const plnt::GOComponentGetter&) override;
	void CollisionHandler(const plnt::EACollisionWithCollider2D& e);
};
PE_GAMEOBJECTCOMPONENT_CLASS(CDamageAdder);
