#pragma once

#include "boost/signals2/signal.hpp"

#include "planeta/PlanetaEngine.hpp"

#include "EADamage.hpp"

/*! ダメージを受けるコンポーネント

	CDamageAdderとセットで使う。"DamageReceiver"ラベルが設定されているコライダーと"DamageAdder"ラベルが設定されているコライダーが衝突するとイベントを発生する。
*/
class CDamageReceiver final: public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDamageReceiver);
public:
	using Super = plnt::GameObjectStandardComponent;
	/*! ダメージを受けるかどうか取得する*/
	bool is_damage_receivable()const;
	/*! ダメージを受けるかどうか設定する*/
	void is_damage_receivable(bool f);
	/*! ダメージを与える。基本的にCDamageAdderにより呼び出されるので、直接呼ぶことはない。*/
	void AddDamage(int damage, int invincible_frame);
	/*! ダメージを受けたときのイベント型*/
	using DamageReceivedEventType = boost::signals2::signal<void(const EADamageReceived&)>;
	/*! ダメージを受けたときのイベント*/
	DamageReceivedEventType damage_received;
	/*! 無敵時間が終了した時のイベント型*/
	using InvincibleTimeEndedEventType = boost::signals2::signal<void()>;
	/*! 無敵時間が終了した時のイベント*/
	InvincibleTimeEndedEventType invincible_time_ended;
private:
	bool is_damage_receivable_ = true;
	int invincible_frame_counter_ = 0;
	void OnInitialized()override;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CDamageReceiver);
