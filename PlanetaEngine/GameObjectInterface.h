#pragma once

namespace planeta {
	class IGameObject;
	//���`�p����������邽�߁A�e���v���[�g�ɂ��ʂ̌^�Ƃ���B(�q�N���X�ɉ��z�p�����Ă��炤���@�ł́Avirtual�̎w��Y�ꂪ��������\�������邽�߁B)
	template<class GIType>
	class GameObjectInterface {
	public:
		IGameObject& game_object;
	protected:
		GameObjectInterface(IGameObject& p_game_object) :game_object(p_game_object) {}
	};
}