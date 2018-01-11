#pragma once

#include "planeta/core/GameObjectSystemComponent.hpp"

namespace plnt {
	class ScreenDrawerGUI;
	class CDrawGUI : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGUI);
	public:
		using Super = private_::GameObjectSystemComponent;
		CDrawGUI();
		virtual ~CDrawGUI();
		/*�`�揈��*/
		void Draw(ScreenDrawerGUI& drawer);
		/*! @brief �`��D��x���擾

		�l���傫���قǎ�O�ɕ\�������*/
		int draw_priority()const;
		/*! @brief �`��D��x��ݒ�

		�l���傫���قǎ�O�ɕ\�������*/
		CDrawGUI& draw_priority(int priority);
	protected:
		virtual void OnInitialized()override;
		virtual void OnFinalized()noexcept override;
		virtual void OnActivated()override;
		virtual void OnInactivated()override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		virtual void DrawProc(ScreenDrawerGUI& drawer) = 0;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGUI);
}
