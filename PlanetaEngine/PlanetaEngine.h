#pragma once
#include <memory>

//���C�u�����t�@�C���̎w��
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <functional>
#include "Object.h"
#include "SharedPointerInstance.h"

namespace planeta_engine {
	namespace core {
		class IGameSetUp;
		class Game;
		/*�Q�[���N���X�͂��̃N���X���p�����A���������`����B*/
		class PlanetaEngine : public core::Object,public utility::SharedPointerInstance<PlanetaEngine>{
		public:
			PlanetaEngine();
			~PlanetaEngine();
			/*�G���W���̏�����(���s�����ꍇ�ł��I���������Ăяo���K�v������)*/
			bool Initialize();
			/*�G���W���̏I������*/
			void Finalize();
			/*�G���W���̃X�e�[�^�X*/
			enum class Status { Continue, Quit, Error };
			/*�G���W���̍X�V*/
			Status Update();
		protected:
			IGameSetUp& game_setup_access();
		private:
			PlanetaEngine(const PlanetaEngine&) = delete;
			PlanetaEngine(PlanetaEngine&&) = delete;
			PlanetaEngine& operator=(const PlanetaEngine&) = delete;
			PlanetaEngine& operator=(PlanetaEngine&&) = delete;
			bool _is_initialized;
			std::unique_ptr<Game> _game;
			//�Q�[���ŗL�̏�����
			virtual bool _InitializeGame() = 0;
			//�X�^�[�g�A�b�v�V�[��
			virtual std::string _StartUpSceneName()const = 0;
		};
	}
}
