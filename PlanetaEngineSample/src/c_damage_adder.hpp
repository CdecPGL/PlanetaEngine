#pragma once

#include "planeta/planeta_engine.hpp"

#include "e_damage.hpp"

/*! ダメージを与えるコンポーネント

CDamageReceiverとセットで使う。"DamageReceiver"ラベルが設定されているコライダーと"DamageAdder"ラベルが設定されているコライダーが衝突すると、DamageReceiverの方のCdamageReceiver::AddDamageを呼び出す。
*/
class c_damage_adder final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_damage_adder);

public:
	using super = game_object_standard_component;
	/*! 攻撃力を取得する*/
	[[nodiscard]] int power() const;
	/*! 攻撃力を設定する*/
	void power(int p);
	/*! 与える無敵フレーム時間を取得する*/
	[[nodiscard]] int add_invincible_frame() const;
	/*! 与える無敵フレーム時間を設定する*/
	void add_invincible_frame(int inv_f);
	/*! ダメージを与えたときのイベント*/
	plnt::signal<void(const e_damage_added &)> damage_added;

private:
	int add_invincible_frame_ = 1;
	int power_ = 1;
	[[nodiscard]] bool get_other_components_proc(const plnt::go_component_getter &) override;
	void collision_handler(const plnt::e_collision_with_collider_2d &e);
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_damage_adder);
