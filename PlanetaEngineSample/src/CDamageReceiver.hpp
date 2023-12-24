#pragma once

#include "boost/signals2/signal.hpp"

#include "planeta/PlanetaEngine.hpp"

#include "EADamage.hpp"

/*! �_���[�W���󂯂�R���|�[�l���g

	CDamageAdder�ƃZ�b�g�Ŏg���B"DamageReceiver"���x�����ݒ肳��Ă���R���C�_�[��"DamageAdder"���x�����ݒ肳��Ă���R���C�_�[���Փ˂���ƃC�x���g�𔭐�����B
*/
class CDamageReceiver final: public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDamageReceiver);
public:
	using Super = plnt::GameObjectStandardComponent;
	/*! �_���[�W���󂯂邩�ǂ����擾����*/
	bool is_damage_receivable()const;
	/*! �_���[�W���󂯂邩�ǂ����ݒ肷��*/
	void is_damage_receivable(bool f);
	/*! �_���[�W��^����B��{�I��CDamageAdder�ɂ��Ăяo�����̂ŁA���ڌĂԂ��Ƃ͂Ȃ��B*/
	void AddDamage(int damage, int invincible_frame);
	/*! �_���[�W���󂯂��Ƃ��̃C�x���g�^*/
	using DamageReceivedEventType = boost::signals2::signal<void(const EADamageReceived&)>;
	/*! �_���[�W���󂯂��Ƃ��̃C�x���g*/
	DamageReceivedEventType damage_received;
	/*! ���G���Ԃ��I���������̃C�x���g�^*/
	using InvincibleTimeEndedEventType = boost::signals2::signal<void()>;
	/*! ���G���Ԃ��I���������̃C�x���g*/
	InvincibleTimeEndedEventType invincible_time_ended;
private:
	bool is_damage_receivable_ = true;
	int invincible_frame_counter_ = 0;
	void OnInitialized()override;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CDamageReceiver);
