#pragma once


//���C�u�����t�@�C���̎w��
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <memory>
#include "Object.h"
#include "StaticSingletonTemplate.h"

namespace planeta_engine {
	namespace core {
		class Game;
	}
	/*�Q�[���N���X�͂��̃N���X���p�����A���������`����B*/
	class PlanetaEngine final : public utility::StaticSingletonTemplate<PlanetaEngine> {
		friend utility::StaticSingletonTemplate<PlanetaEngine>;
	public:
		/*�Q�[���̍쐬(�������O�ɍs��)*/
		template<class GameType>
		bool CreateGameInstance() {
			game_ = std::make_unique<GameType>();
			return game_ != nullptr;
		}
		/*�G���W���̏�����*/
		bool Initialize()override;
		/*�G���W���̏I������*/
		void Finalize()override;
		/*�G���W���̃X�e�[�^�X*/
		enum class Status { Continue, Quit, Error };
		/*�G���W���̍X�V(������������ɍs���Ă��Ȃ���Ԃł̌Ăяo���͖���`����)*/
		Status Update();
	private:
		PlanetaEngine();
		~PlanetaEngine();
		PlanetaEngine(const PlanetaEngine&) = delete;
		PlanetaEngine(PlanetaEngine&&) = delete;
		PlanetaEngine& operator=(const PlanetaEngine&) = delete;
		PlanetaEngine& operator=(PlanetaEngine&&) = delete;
		bool is_initialized_;
		std::unique_ptr<core::Game> game_;

		bool InitializeEngine();
		void FinalzieEngine();

		bool InitializeDebugSystem();
		void FinalizeDebugSystem();
	};
}
