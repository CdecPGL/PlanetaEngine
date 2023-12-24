#pragma once

#include "planeta/PlanetaEngine.hpp"

#include "EADamage.hpp"

/*! ダメージを与えるコンポーネント

CDamageReceiverとセットで使う。"DamageReceiver"ラベルが設定されているコライダーと"DamageAdder"ラベルが設定されているコライダーが衝突すると、DamageReceiverの方のCdamageReceiver::AddDamageを呼び出す。
*/
class CDamageAdder final: public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDamageAdder);
public:
	using Super = plnt::GameObjectStandardComponent;
	/*! 攻撃力を取得する*/
	int power()const;
	/*! 攻撃力を設定する*/
	void power(int p);
	/*! 与える無敵フレーム時間を取得する*/
	int add_invincible_frame()const;
	/*! 与える無敵フレーム時間を設定する*/
	void add_invincible_frame(int inv_f);
	/*! ダメージを与えたときのイベント*/
	plnt::Signal<void(const EADamageAdded&)> damage_added;
private:
	int add_invincible_frame_ = 1;
	int power_ = 1;
	bool GetOtherComponentsProc(const plnt::GOComponentGetter&) override;
	void CollisionHandler(const plnt::EACollisionWithCollider2D& e);
};
PE_GAMEOBJECTCOMPONENT_CLASS(CDamageAdder);
