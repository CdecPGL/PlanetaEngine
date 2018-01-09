#pragma once

#include "GameObjectStandardComponent.hpp"
#include "NonOwingPointer.hpp"

namespace planeta {
	class CEffect final: public GameObjectStandardComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CEffect);
	public:
		using Super = GameObjectStandardComponent;
		CEffect();
		~CEffect();
		/*! �G�t�F�N�g���Đ�����*/
		bool Play();
		/*! �G�t�F�N�g���~����*/
		bool Stop();
		/*! �G�t�F�N�g���\�[�XID��ݒ肷��*/
		void resource_id(const std::string& resource_id);
		/*! �G�t�F�N�g���Đ������m�F*/
		bool is_playing()const;
		/*! ���[�v���邩�̃t���O��ݒ�*/
		void roop_flag(bool f);
		/*! ���[�v���邩�̃t���O���擾*/
		bool roop_flag()const;
		/*! �����Đ����邩�ǂ����̃t���O��ݒ�*/
		void auto_play(bool f);
		/*! �����Đ����邩�ǂ����̃t���O���擾*/
		bool auto_play()const;
		/*! �G�t�F�N�g�̊g�嗦��ݒ�*/
		void expansion(double e);
		/*! �G�t�F�N�g�̊g�嗦���擾*/
		double expansion()const;
	protected:
		virtual bool GetOtherComponentsProc(const GOComponentGetter&) override;
		virtual void OnInitialized() override;
		virtual void OnActivated() override;
		virtual void OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CEffect);
}