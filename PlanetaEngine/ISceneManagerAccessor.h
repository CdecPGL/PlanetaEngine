#pragma once

#include <string>
#include "ParameterHolder.h"

namespace planeta_engine {
	namespace core {
		class ISceneManagerAccessor {
		public: 
			virtual ~ISceneManagerAccessor() = 0 {};
			/*次シーンを読み込む(遷移はしない)*/
			virtual bool LoadNextScene(const std::string& scene_id) = 0;
			/*読み込み進行度取得(0.0-1.0。未読み込み時には0.0)*/
			virtual double GetLoadProgress()const = 0;
			/*読み込み中か*/
			virtual bool IsLoading()const = 0;
			/*遷移中か*/
			virtual bool IsTransitioning()const = 0;
			/*遷移可能か*/
			virtual bool IsTransitionable()const = 0;
			/*次のシーンIDを取得*/
			virtual const std::string GetNextSceneID()const = 0;
			/*現在のシーンIDを取得*/
			virtual const std::string GetCurrentSceneID()const = 0;
			/*シーンの遷移(読み込みが必要。読み込み中だったら読み込みつつ遷移)*/
			virtual bool TransitionScene(const utility::ParameterHolder& transition_parameters) = 0;
			/*シーンを読み込んで遷移する*/
			virtual bool LoadAndTransitionScene(const std::string& scene_id, const utility::ParameterHolder& transition_parameters = utility::ParameterHolder()) = 0;
			/*終了*/
			virtual void QuitScene() = 0;
			/*エラー発生*/
			virtual void ErrorOccured() = 0;
		};
	}
}
