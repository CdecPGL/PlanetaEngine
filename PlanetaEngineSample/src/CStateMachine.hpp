#pragma once

#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <list>

#include "boost/optional.hpp"

#include "planeta/PlanetaEngine.hpp"


/*! @brief 状態遷移マシーンコンポーネント

	GameObjectの初期化関数内かコンポーネント追加関数で、ステートの設定を行う。
	遷移時に子ステートまで指定した場合は、途中のEnetr関数の戻り値は無視される。

	@todo エラーメッセージでステートパスを出力するようにする。ステートパスによるマップを保持しておいて、パスによる検索を高速化する。
*/

namespace plnt {
	class IGameObject;
}
class CStateMachine : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CStateMachine);
private:
	class StateBase {
	public:
		virtual ~StateBase() = default;
		virtual boost::optional<std::string> EnterProcBase() = 0;
		virtual boost::optional<std::string> UpdateProcBase() = 0;
		virtual void ExitProcBase() = 0;
		virtual boost::optional<std::string> MessageHandlerBase(const std::string&) = 0;
	};
public:
	using Super = plnt::GameObjectStandardComponent;
	CStateMachine();
	template<class S>
	bool RegisterState(const std::string& state_path, typename S::ArgType& arg_com);
	/*! ステートの基本となるクラス。テンプレート引数にはステート関数の引数となるコンポーネント型を指定する*/
	template<class C>
	class State : public StateBase {
		template<class S>
		friend bool CStateMachine::RegisterState(const std::string&, typename S::ArgType&);
	public:
		using ArgType = C;
		virtual ~State() = default;
		/*! @brief ステートの開始時に呼ばれる関数。
			
			戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> EnterProc(C&) { return boost::none; };
		/*! @brief ステートの更新時に呼ばれる関数。

		戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> UpdateProc(C&) { return boost::none; };
		/*! @brief ステートの終了時に呼ばれる関数。*/
		virtual void ExitProc(C&) {};
		/*! @brief メッセージを受け取ったときに呼ばれる関数。

		戻り値で遷移先のステートを親を基準とする相対パスで指定可能*/
		virtual boost::optional<std::string> MessageHandler(C&, const std::string&) { return boost::none; };
	private:
		C* arg_com_ = nullptr;
		void SetArgCom_(C& arg_com) {
			arg_com_ = &arg_com;
		}

		boost::optional<std::string> EnterProcBase()final {
			return EnterProc(*arg_com_);
		}
		boost::optional<std::string> UpdateProcBase()final {
			return UpdateProc(*arg_com_);
		}
		void ExitProcBase()final {
			ExitProc(*arg_com_);
		}
		boost::optional<std::string> MessageHandlerBase(const std::string& mess)final {
			return MessageHandler(*arg_com_, mess);
		}
	};
	//!遷移(.区切りで、子ステートにもアクセス可能)
	bool Transition(const std::string& state_path);
	//!メッセージを送信
	bool SendMessage(const std::string& message);
	//!ステートマシンを開始(.区切りで、子ステートにもアクセス可能)
	bool StartStateMachine(const std::string& state_path);
	//!ステートマシンを停止
	void StopStateMachine();
private:
	void OnInitialized()override;

	struct StateData;
	//ステート階層構造内でのインデックス-1をインデックスとした実行中ステートのリスト。ルートステートは含まれない。
	std::vector<std::pair<size_t, std::shared_ptr<StateBase>>> active_state_list_;
	//登録されたステートの管理用データ
	struct StateData {
		std::string state_id; //ステートID
		std::function<std::shared_ptr<StateBase>()> creator; //ステートクリエータ
		size_t parent_state_data_idx; //親ステートのインデックス
		std::unordered_map<std::string, size_t> child_state_data_idxes; //子ステートのインデックス
		size_t state_hierarchy_depth; //ルートステートを0とした階層構造内での深度
	};
	//ステートIDとインデックスのマップ(コメント解除するなら、キーをステートパスで持つべき。そのために、ステートデータは捨て０とパスも保持しておくべき)
	//std::unordered_map<std::string, size_t> state_data_map_;
	//ステートデータリスト。インデックス0はルートステート
	std::vector<StateData> state_data_list_;

	bool RegisterState_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_path);
	bool RegisterStateWithParentStateDataIdx_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_id, size_t parent_state_data_idx);
	 //引数はステートパス
	bool Transition_(const std::string& state_path);
	bool ParentBaseTransition_(const StateData& parent_state_data, std::list<std::string>& ids);
	bool MeBaseTransition_(const StateData& state_data, const std::string& relative_state_path);
	void Update_();
};
PE_GAMEOBJECTCOMPONENT_CLASS(CStateMachine);

//! ステートを登録(引数となるゲームオブジェクトコンポーネントはこのコンポーネントと同じGameObjectに保持されていなければならない。)
template<class S>
bool CStateMachine::RegisterState(const std::string& state_path, typename S::ArgType& arg_com) {
	//テンプレートパラメータがStateBaseを継承しているか確認
	static_assert(std::is_base_of_v<StateBase, S>, "The template paremeter S must derive StateBase");
	//登録されたGOCのGameObjectと自分のゲームオブジェクトが同じものを指しているか確認
	PE_VERIFY(&game_object() == &arg_com.game_object());
	//このコンポーネントの寿命とarg_comの参照先の寿命は同じである(同じGameObjectのコンポーネントより)ことを前提とする
	return RegisterState_([&arg_com] {auto s = std::make_shared<S>(); s->SetArgCom_(arg_com); return s; }, state_path);
}
