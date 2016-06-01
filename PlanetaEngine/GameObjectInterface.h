#pragma once

namespace planeta {
	class IGameObject;
	//楔形継承を回避するため、テンプレートにより別の型とする。(子クラスに仮想継承してもらう方法では、virtualの指定忘れが発生する可能性があるため。)
	template<class GIType>
	class GameObjectInterface {
	public:
		IGameObject& game_object;
	protected:
		GameObjectInterface(IGameObject& p_game_object) :game_object(p_game_object) {}
	};
}