#pragma once

#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <list>

#include "planeta/planeta_engine.hpp"


/*! @brief 状態遷移マシーンコンポーネント

	GameObjectの初期化関数内かコンポーネント追加関数で、ステートの設定を行う。
	遷移時に子ステートまで指定した場合は、途中のEnetr関数の戻り値は無視される。

	@todo エラーメッセージでステートパスを出力するようにする。ステートパスによるマップを保持しておいて、パスによる検索を高速化する。
*/

namespace plnt {
	class i_game_object;
}

class c_state_machine final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_state_machine);

private:
	class state_base {
	public:
		virtual ~state_base() = default;
		virtual boost::optional<std::string> enter_proc_base() = 0;
		virtual boost::optional<std::string> update_proc_base() = 0;
		virtual void exit_proc_base() = 0;
		virtual boost::optional<std::string> message_handler_base(const std::string &) = 0;
	};

public:
	using super = game_object_standard_component;
	c_state_machine();
	template <class S>
	bool register_state(const std::string &state_path, typename S::arg_type &arg_com);

	/*! ステートの基本となるクラス。テンプレート引数にはステート関数の引数となるコンポーネント型を指定する*/
	template <class C>
	class state : public state_base {
		template <class S>
		friend bool c_state_machine::register_state(const std::string &, typename S::arg_type &);

	public:
		using arg_type = C;
		~state() override = default;
		/*! @brief ステートの開始時に呼ばれる関数。
			
			戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> enter_proc(C &) { return boost::none; }
		/*! @brief ステートの更新時に呼ばれる関数。

		戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> update_proc(C &) { return boost::none; }
		/*! @brief ステートの終了時に呼ばれる関数。*/
		virtual void exit_proc(C &) {}
		/*! @brief メッセージを受け取ったときに呼ばれる関数。

		戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> message_handler(C &, const std::string &) { return boost::none; }

	private:
		C *arg_com_ = nullptr;
		void set_arg_com(C &arg_com) { arg_com_ = &arg_com; }

		boost::optional<std::string> enter_proc_base() final { return enter_proc(*arg_com_); }
		boost::optional<std::string> update_proc_base() final { return update_proc(*arg_com_); }
		void exit_proc_base() final { exit_proc(*arg_com_); }

		boost::optional<std::string> message_handler_base(const std::string &mess) final {
			return message_handler(*arg_com_, mess);
		}
	};

	//!遷移(.区切りで、子ステートにもアクセス可能)
	bool transition(const std::string &state_path);
	//!メッセージを送信
	bool send_message(const std::string &message);
	//!ステートマシンを開始(.区切りで、子ステートにもアクセス可能)
	bool start_state_machine(const std::string &state_path);
	//!ステートマシンを停止
	void stop_state_machine();

private:
	void on_initialized() override;

	struct state_data;
	//ステート階層構造内でのインデックス-1をインデックスとした実行中ステートのリスト。ルートステートは含まれない。
	std::vector<std::pair<size_t, std::shared_ptr<state_base>>> active_state_list_;

	//登録されたステートの管理用データ
	struct state_data {
		std::string state_id; //ステートID
		std::function<std::shared_ptr<state_base>()> creator; //ステートクリエータ
		size_t parent_state_data_idx; //親ステートのインデックス
		std::unordered_map<std::string, size_t> child_state_data_idxes; //子ステートのインデックス
		size_t state_hierarchy_depth; //ルートステートを0とした階層構造内での深度
	};

	//ステートIDとインデックスのマップ(コメント解除するなら、キーをステートパスで持つべき。そのために、ステートデータは捨て０とパスも保持しておくべき)
	//std::unordered_map<std::string, size_t> state_data_map_;
	//ステートデータリスト。インデックス0はルートステート
	std::vector<state_data> state_data_list_;

	bool register_state(const std::function<std::shared_ptr<state_base>()> &creator, const std::string &state_path);
	bool register_state_with_parent_state_data_idx(const std::function<std::shared_ptr<state_base>()> &creator,
	                                          const std::string &state_id, size_t parent_state_data_idx);
	//引数はステートパス
	bool transition_impl(const std::string &state_path);
	bool parent_base_transition(const state_data &parent_state_data, std::list<std::string> &ids);
	bool me_base_transition(const state_data &state_data, const std::string &relative_state_path);
	void update();
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_state_machine);

//! ステートを登録(引数となるゲームオブジェクトコンポーネントはこのコンポーネントと同じGameObjectに保持されていなければならない。)
template <class S>
bool c_state_machine::register_state(const std::string &state_path, typename S::arg_type &arg_com) {
	//テンプレートパラメータがStateBaseを継承しているか確認
	static_assert(std::is_base_of_v<state_base, S>, "The template parameter S must derive StateBase");
	//登録されたGOCのGameObjectと自分のゲームオブジェクトが同じものを指しているか確認
	PE_VERIFY(&game_object() == &arg_com.game_object());
	//このコンポーネントの寿命とarg_comの参照先の寿命は同じである(同じGameObjectのコンポーネントより)ことを前提とする
	return register_state([&arg_com] {
		auto s = std::make_shared<S>();
		s->set_arg_com(arg_com);
		return s;
	}, state_path);
}
