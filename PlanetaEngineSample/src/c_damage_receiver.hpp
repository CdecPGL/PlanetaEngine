#pragma once

#include "boost/signals2/signal.hpp"

#include "planeta/planeta_engine.hpp"

#include "e_damage.hpp"

/*! ダメージを受けるコンポーネント

	CDamageAdderとセットで使う。"DamageReceiver"ラベルが設定されているコライダーと"DamageAdder"ラベルが設定されているコライダーが衝突するとイベントを発生する。
*/
class c_damage_receiver final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_damage_receiver);

public:
	using super = game_object_standard_component;
	/*! ダメージを受けるかどうか取得する*/
	[[nodiscard]] bool is_damage_receivable() const;
	/*! ダメージを受けるかどうか設定する*/
	void is_damage_receivable(bool f);
	/*! ダメージを与える。基本的にCDamageAdderにより呼び出されるので、直接呼ぶことはない。*/
	void add_damage(int damage, int invincible_frame);
	/*! ダメージを受けたときのイベント型*/
	using damage_received_event_type = boost::signals2::signal<void(const e_damage_received &)>;
	/*! ダメージを受けたときのイベント*/
	damage_received_event_type damage_received;
	/*! 無敵時間が終了した時のイベント型*/
	using invincible_time_ended_event_type = boost::signals2::signal<void()>;
	/*! 無敵時間が終了した時のイベント*/
	invincible_time_ended_event_type invincible_time_ended;

private:
	bool is_damage_receivable_ = true;
	int invincible_frame_counter_ = 0;
	void on_initialized() override;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_damage_receiver);
