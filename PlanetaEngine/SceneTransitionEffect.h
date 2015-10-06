#pragma once

namespace planeta_engine{
	namespace core{
		class SceneTransitionEffect{
		public:
			SceneTransitionEffect();
			virtual ~SceneTransitionEffect();
			enum class EffectState{ Skip, Finished, Playing }; //�G�t�F�N�g�̏��(Skip:�G�t�F�N�g�͍Đ����Ȃ��AFinished:�G�t�F�N�g�Đ����I������APlaying:�G�t�F�N�g�Đ���)
			enum class SceneProgressPermission{ Permitting,NotPermitting }; // �V�[���̐i�s����(���A�s����)
			/*�J�ڃG�t�F�N�g�֐��B
			FadeOutEffect,WaitForLoad,TransitionEffect,FadeInEffect�̏��ɌĂ΂��B
			FadeOut,FadeIn���̓V�[���͐i�s����B
			Transitioning���̓V�[���͐i�s���Ȃ��B
			FadeOut,Transition,FadeIn�̏I���̓G�t�F�N�g���͐���B�߂�l�ŏI����m�点��B�I������Ǝ��Ɉڍs�B
			Loading�͓ǂݍ��ݑҋ@���ɂ̂݌Ă΂��B(�ǂݍ��݂��I����葁���t�F�[�h�A�E�g���I�������ꍇ)
			�I���͊O������ŁA�߂�l�ŃG�t�F�N�g���̃V�[���i�s�̉ۂ�Ԃ�*/
			virtual EffectState FadeOutEffect(){ return EffectState::Skip; }; //���݂̃V�[������t�F�[�h�A�E�g
			virtual SceneProgressPermission WaitForLoadEffect(){ return SceneProgressPermission::Permitting; } //�ǂݍ��ݑҋ@���ɌĂ΂��
			virtual EffectState TransitionEffect(){ return EffectState::Skip; }; //�J�ڃG�t�F�N�g
			virtual EffectState FadeInEffect(){ return EffectState::Skip; }; //�V�����V�[���Ƀt�F�[�h�C��
		protected:

		private:

		};
	}
}