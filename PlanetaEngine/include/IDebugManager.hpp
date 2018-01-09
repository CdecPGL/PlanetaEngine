#pragma once

#include <functional>
#include "StringUtility.hpp"
#include "Vector2D.hpp"
#include "Color.hpp"

namespace planeta {
	/*! �f�o�b�O���ǉ��N���X���J�C���^�[�t�F�C�X*/
	class IDebugInformationAdder {
	public:
		virtual ~IDebugInformationAdder() = 0 {};
		/*! ������ōs��ǉ�����*/
		virtual void AddLine(const std::string line) = 0;
		/*! �C�ӂ̈����ōs��ǉ�����*/
		template<typename... Args>
		void AddLineV(Args&&... args) {
			AddLine(util::ConvertAndConnectToString(args...));
		}
	};
	/*! �f�o�b�O�`��N���X���J�C���^�[�t�F�C�X*/
	class IDebugDrawer {
	public:
		virtual ~IDebugDrawer() = 0 {};
		/*! ����`�悷��*/
		virtual void DrawLine(const Vector2Df& spos, const Vector2Df& epos, const Color& color) = 0;
		/*! �~��`�悷��*/
		virtual void DrawCircle(const Vector2Df& pos, float radius, const Color& color, bool filled) = 0;
		/*! �����̐���`�悷��*/
		virtual void DrawLines(const std::vector<Vector2Df>& pos_list, const Color& color) = 0;
	};
	/*! �f�o�b�O�}�l�[�W�����J�C���^�[�t�F�C�X*/
	class IDebugManager {
	public:
		virtual ~IDebugManager() = 0 {};
		/*! �f�o�b�O���`�����l�����쐬����*/
		virtual bool CreateDebugInformationChannel(const std::string& channel_id,const std::function<void(IDebugInformationAdder&)>& handler) = 0;
		/*! �f�o�b�O���`�����l�����폜����*/
		virtual bool DeleteDebugInformationChannel(const std::string& channel_id) = 0;
		/*! �f�o�b�O�`��p�`�����l�����쐬����*/
		virtual bool CreateDebugDrawChannel(const std::string& channel_id,const std::function<void(IDebugDrawer&)> handler) = 0;
		/*! �f�o�b�O�`��p�`�����l�����폜����*/
		virtual bool DeleteDebugDrawChannel(const std::string& channel_id) = 0;
	
	};
}