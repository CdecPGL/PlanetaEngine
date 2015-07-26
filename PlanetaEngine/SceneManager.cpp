#include "SceneManager.h"
#include "Scene.h"
#include "SceneDefinition.h"
#include "SceneTransitionEffect.h"

namespace PlanetaEngine{
	namespace Core{
		
		SceneManager::SceneStatus SceneManager::Process()
		{
			if (_scene_progress_flag){ _current_scene->Update(); }
			_transition_proc();
			return SceneStatus::Continue;
		}

		bool SceneManager::PrepairScene(const std::string& scene_name){
			if (_state != SceneManageState::Playing){ return false; } //�J�ډ\�ȏ�ԂłȂ�
			return _prepair_controller->Prepair(scene_name);
		}

		bool SceneManager::TransitionScene(const std::string& scene_name, const std::string& effect_name/*= "\0"*/)
		{
			if (_state != SceneManageState::Playing){ return false; } //�V�[���J�ڒ��̂��ߐV���ɑJ�ڏ������n�߂邱�Ƃ͂ł��Ȃ�
			if (_prepair_controller->Prepair(scene_name) == false){ return false; }; //�V�[���̏������ł��Ȃ�����
			//�w�薼�̃V�[����`�����݂��邩�m�F
			auto it = _scene_definition_creaters.find(scene_name);
			if (it == _scene_definition_creaters.end()){ return false; }
			else{
				_next_scene_definition = std::move(it->second());
			}
			//�w�薼�̃G�t�F�N�g�����݂��邩�m�F
			auto eit = _scene_transition_effect_creaters.find(effect_name);
			if (eit == _scene_transition_effect_creaters.end()){
				//�w�肳�ꂽ�V�[���ύX�G�t�F�N�g�����݂��Ȃ�

			}
			else{
				_transition_effect = std::move(eit->second());
			}
			return true;
		}

		void SceneManager::_transition_proc()
		{
			if (_transition_effect){ //�J�ڏ�������������
				bool transition_continue = true; //�J�ڏ����𑱂��邩
				while (transition_continue){
					switch (_transition_state)
					{
					case PlanetaEngine::Core::SceneManager::FadeOutCurrentScene: //�t�F�[�h�A�E�g
						//�G�t�F�N�g���Đ���ԂłȂ���Ύ��Ɉڂ�
						if (_transition_effect->FadeOutEffect() != SceneTransitionEffect::EffectState::Playing){ _transition_state = true ? _TransitionState::WaitLoading : _TransitionState::Transition; }
						//�G�t�F�N�g���X�L�b�v��Ԃ�������A���Ɉڂ�A����ɂ�����x�J�ڏ������s��
						if (_transition_effect->FadeOutEffect() == SceneTransitionEffect::EffectState::Skip){ transition_continue = true; }
						else{ transition_continue = false; }
						//�V�[���i�s�͋���
						_scene_progress_flag = true;
						break;
					case PlanetaEngine::Core::SceneManager::WaitLoading: //�ǂݍ��ݑҋ@
						//�����ł����玟��
						if (_prepair_controller->GetProgress() == _PrepairController::PrepairProgress::Ready){
							_transition_state = Transition;
							transition_continue = true;
						}
						else{ //�ł��ĂȂ�������ҋ@�G�t�F�N�g�Đ�
							//�G�t�F�N�g�������s���A���̌��ʂɂ���ăV�[���i�s�̉ۂ����߂�
							_scene_progress_flag = _transition_effect->WaitForLoadEffect() == SceneTransitionEffect::SceneProgressPermission::Permitting;
							transition_continue = false;
						}
						break;
					case PlanetaEngine::Core::SceneManager::Transition:
						//�J�ڃG�t�F�N�g���I�������玟��
						if (_transition_effect->TransitionEffect() == SceneTransitionEffect::EffectState::Finished){ _transition_state = _TransitionState::FadeInNewScene; }
						//�J�ڃG�t�F�N�g���X�L�b�v�������玟�ɐi�݂�����x�J�ڏ������s���B
						if (_transition_effect->FadeOutEffect() == SceneTransitionEffect::EffectState::Skip){ transition_continue = true; }
						else{ transition_continue = false; }
						//�V�[���i�s�͕s����
						_scene_progress_flag = false;
						break;
					case PlanetaEngine::Core::SceneManager::FadeInNewScene:
						//�t�F�[�h�C���G�t�F�N�g���I�������J�ڏI��
						if (_transition_effect->FadeInEffect() == SceneTransitionEffect::EffectState::Finished){
							//�J�ڏ����I��
							_transition_state = _TransitionState::NotTransitioning;
							_transition_effect.reset();
						}
						transition_continue = false;
						//�V�[���i�s�͋���
						_scene_progress_flag = true;
						break;
					case PlanetaEngine::Core::SceneManager::NotTransitioning:
						//�����ɂ͗��Ȃ��͂�
						break;
					default:
						//���������Ȃ�
						break;
					}
				}
			}
			else{ //�J�ڏ������s���Ă��Ȃ�������
				//�V�[���ύX���N�G�X�g����������
				if (_request.is_requested){
					auto it = _scene_definition_creaters.find(_request.scene_name);
					if (it == _scene_definition_creaters.end()){ _request.is_requested = false; } //���N�G�X�g���ꂽ�V�[����`�����݂��Ȃ�
					else{
						_next_scene_definition = std::move(it->second()); //�V�[����`�N���X���쐬
						auto eit = _scene_transition_effect_creaters.find(_request.effect);
						if (eit == _scene_transition_effect_creaters.end()){} //���N�G�X�g���ꂽ�J�ڃG�t�F�N�g�����݂��Ȃ�
						else{
							_transition_effect = std::move(eit->second()); //�V�[���J�ڃG�t�F�N�g���쐬
							_transition_state = _TransitionState::FadeInNewScene; //�J�ڏ�Ԃ�ݒ�
						}
					}
				}
			}
		}

		/**********_PrepairController**********/
		bool SceneManager::_PrepairController::Prepair(const std::string& scene_name){
			if (_progress == PrepairProgress::Ready && _scene == scene_name){ return true; } //�w�肳�ꂽ�V�[���̏����͂��łɊ������Ă���B]
			if (_progress == PrepairProgress::Loading){ //�V�[���ǂݍ��ݒ���������
				if (_scene == scene_name){ return true; } //�w�肳�ꂽ�V�[���͂��łɏ�����
				else{ //���̃V�[���̏��������񂱂����Ă���
					//���ݐi�s���Ă���V�[���̏������~

				}
			}
			//�V�[���̏������J�n
			auto it = sm._scene_definition_creaters.find(scene_name);
			if (it == sm._scene_definition_creaters.end()){ return false; } //�w�肳�ꂽ�V�[����`�����݂��Ȃ�
			else{
				//�ǂݍ��ݏ����J�n
				_scene = scene_name;
				//////////////////
				_progress = PrepairProgress::Loading;
				return true;
			}
		}
		
	}
}