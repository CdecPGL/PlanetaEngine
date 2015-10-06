#pragma once

namespace planeta_engine{
	namespace core{
		class SceneTransitionEffect{
		public:
			SceneTransitionEffect();
			virtual ~SceneTransitionEffect();
			enum class EffectState{ Skip, Finished, Playing }; //エフェクトの状態(Skip:エフェクトは再生しない、Finished:エフェクト再生が終わった、Playing:エフェクト再生中)
			enum class SceneProgressPermission{ Permitting,NotPermitting }; // シーンの進行許可(許可、不許可)
			/*遷移エフェクト関数。
			FadeOutEffect,WaitForLoad,TransitionEffect,FadeInEffectの順に呼ばれる。
			FadeOut,FadeIn中はシーンは進行する。
			Transitioning中はシーンは進行しない。
			FadeOut,Transition,FadeInの終了はエフェクト側は制御。戻り値で終了を知らせる。終了すると次に移行。
			Loadingは読み込み待機時にのみ呼ばれる。(読み込みが終わるより早くフェードアウトが終了した場合)
			終了は外部制御で、戻り値でエフェクト中のシーン進行の可否を返す*/
			virtual EffectState FadeOutEffect(){ return EffectState::Skip; }; //現在のシーンからフェードアウト
			virtual SceneProgressPermission WaitForLoadEffect(){ return SceneProgressPermission::Permitting; } //読み込み待機時に呼ばれる
			virtual EffectState TransitionEffect(){ return EffectState::Skip; }; //遷移エフェクト
			virtual EffectState FadeInEffect(){ return EffectState::Skip; }; //新しいシーンにフェードイン
		protected:

		private:

		};
	}
}