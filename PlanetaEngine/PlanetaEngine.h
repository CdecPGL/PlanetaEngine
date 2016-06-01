#pragma once


//���C�u�����t�@�C���̎w��
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <memory>
#include "Object.h"
#include "SingletonTemplate.h"

namespace planeta_engine {
	/*�Q�[���̃X�e�[�^�X*/
	enum class GameStatus { Continue, Quit, Error };
	/*�Q�[���N���X�͂��̃N���X���p�����A���������`����B*/
	class PlanetaEngine final : public util::SingletonTemplate<PlanetaEngine> {
		friend util::SingletonTemplate<PlanetaEngine>;
	public:
		/*�G���W���̏�����*/
		bool Initialize()override;
		/*�G���W���̏I������*/
		void Finalize()override;
		/*�G���W���̍X�V(������������ɍs���Ă��Ȃ���Ԃł̌Ăяo���͖���`����)*/
		GameStatus Update();
	private:
		PlanetaEngine();
		~PlanetaEngine();

		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
