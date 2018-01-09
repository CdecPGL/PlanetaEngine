#pragma once

#include "CDrawGUI.h"
#include "RectAngle.h"

namespace planeta {
	/*! GUI�̈�ɉ摜��`�悷��R���|�[�l���g*/
	class CDrawGraphGUI final: public CDrawGUI {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawGraphGUI);
	public:
		using Super = CDrawGUI;
		CDrawGraphGUI();
		~CDrawGraphGUI();
		/*! �\������摜�̃��\�[�XID��ݒ肷��*/
		bool resource_id(const std::string& res_id);
		/*! �摜��̕`��̈���s�N�Z���P�ʂŎ擾*/
		const RectAnglei& draw_area()const;
		/*! �摜��̕`��̈���s�N�Z���P�ʂŐݒ�*/
		CDrawGraphGUI& draw_area(const RectAnglei& rect);
		/*! �摜�𔽓]�����邩�擾*/
		const bool reverse()const;
		/*! �摜�𔽓]�����邩�ݒ�*/
		CDrawGraphGUI& reverse(bool rev);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void DrawProc(ScreenDrawerGUI& drawer) override;
		bool GetOtherComponentsProc(const GOComponentGetter&) override;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawGraphGUI);
}