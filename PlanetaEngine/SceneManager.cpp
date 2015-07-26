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
			if (_state != SceneManageState::Playing){ return false; } //遷移可能な状態でない
			return _prepair_controller->Prepair(scene_name);
		}

		bool SceneManager::TransitionScene(const std::string& scene_name, const std::string& effect_name/*= "\0"*/)
		{
			if (_state != SceneManageState::Playing){ return false; } //シーン遷移中のため新たに遷移処理を始めることはできない
			if (_prepair_controller->Prepair(scene_name) == false){ return false; }; //シーンの準備ができなかった
			//指定名のシーン定義が存在するか確認
			auto it = _scene_definition_creaters.find(scene_name);
			if (it == _scene_definition_creaters.end()){ return false; }
			else{
				_next_scene_definition = std::move(it->second());
			}
			//指定名のエフェクトが存在するか確認
			auto eit = _scene_transition_effect_creaters.find(effect_name);
			if (eit == _scene_transition_effect_creaters.end()){
				//指定されたシーン変更エフェクトが存在しない

			}
			else{
				_transition_effect = std::move(eit->second());
			}
			return true;
		}

		void SceneManager::_transition_proc()
		{
			if (_transition_effect){ //遷移処理中だったら
				bool transition_continue = true; //遷移処理を続けるか
				while (transition_continue){
					switch (_transition_state)
					{
					case PlanetaEngine::Core::SceneManager::FadeOutCurrentScene: //フェードアウト
						//エフェクトが再生状態でなければ次に移る
						if (_transition_effect->FadeOutEffect() != SceneTransitionEffect::EffectState::Playing){ _transition_state = true ? _TransitionState::WaitLoading : _TransitionState::Transition; }
						//エフェクトがスキップ状態だったら、次に移り、さらにもう一度遷移処理を行う
						if (_transition_effect->FadeOutEffect() == SceneTransitionEffect::EffectState::Skip){ transition_continue = true; }
						else{ transition_continue = false; }
						//シーン進行は許可
						_scene_progress_flag = true;
						break;
					case PlanetaEngine::Core::SceneManager::WaitLoading: //読み込み待機
						//準備できたら次へ
						if (_prepair_controller->GetProgress() == _PrepairController::PrepairProgress::Ready){
							_transition_state = Transition;
							transition_continue = true;
						}
						else{ //できてなかったら待機エフェクト再生
							//エフェクト処理を行い、その結果によってシーン進行の可否を決める
							_scene_progress_flag = _transition_effect->WaitForLoadEffect() == SceneTransitionEffect::SceneProgressPermission::Permitting;
							transition_continue = false;
						}
						break;
					case PlanetaEngine::Core::SceneManager::Transition:
						//遷移エフェクトが終了したら次へ
						if (_transition_effect->TransitionEffect() == SceneTransitionEffect::EffectState::Finished){ _transition_state = _TransitionState::FadeInNewScene; }
						//遷移エフェクトがスキップだったら次に進みもう一度遷移処理を行う。
						if (_transition_effect->FadeOutEffect() == SceneTransitionEffect::EffectState::Skip){ transition_continue = true; }
						else{ transition_continue = false; }
						//シーン進行は不許可
						_scene_progress_flag = false;
						break;
					case PlanetaEngine::Core::SceneManager::FadeInNewScene:
						//フェードインエフェクトが終わったら遷移終了
						if (_transition_effect->FadeInEffect() == SceneTransitionEffect::EffectState::Finished){
							//遷移処理終了
							_transition_state = _TransitionState::NotTransitioning;
							_transition_effect.reset();
						}
						transition_continue = false;
						//シーン進行は許可
						_scene_progress_flag = true;
						break;
					case PlanetaEngine::Core::SceneManager::NotTransitioning:
						//ここには来ないはず
						break;
					default:
						//ここもこない
						break;
					}
				}
			}
			else{ //遷移処理が行われていなかったら
				//シーン変更リクエストがあったら
				if (_request.is_requested){
					auto it = _scene_definition_creaters.find(_request.scene_name);
					if (it == _scene_definition_creaters.end()){ _request.is_requested = false; } //リクエストされたシーン定義が存在しない
					else{
						_next_scene_definition = std::move(it->second()); //シーン定義クラスを作成
						auto eit = _scene_transition_effect_creaters.find(_request.effect);
						if (eit == _scene_transition_effect_creaters.end()){} //リクエストされた遷移エフェクトが存在しない
						else{
							_transition_effect = std::move(eit->second()); //シーン遷移エフェクトを作成
							_transition_state = _TransitionState::FadeInNewScene; //遷移状態を設定
						}
					}
				}
			}
		}

		/**********_PrepairController**********/
		bool SceneManager::_PrepairController::Prepair(const std::string& scene_name){
			if (_progress == PrepairProgress::Ready && _scene == scene_name){ return true; } //指定されたシーンの準備はすでに完了している。]
			if (_progress == PrepairProgress::Loading){ //シーン読み込み中だったら
				if (_scene == scene_name){ return true; } //指定されたシーンはすでに準備中
				else{ //他のシーンの準備がしんこうしている
					//現在進行しているシーンの準備を停止

				}
			}
			//シーンの準備を開始
			auto it = sm._scene_definition_creaters.find(scene_name);
			if (it == sm._scene_definition_creaters.end()){ return false; } //指定されたシーン定義が存在しない
			else{
				//読み込み処理開始
				_scene = scene_name;
				//////////////////
				_progress = PrepairProgress::Loading;
				return true;
			}
		}
		
	}
}