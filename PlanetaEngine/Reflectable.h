#pragma once

#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

#include "Reflection.h"

namespace planeta {
	/*! Reflection���N���X�B���t���N�V�����ɑΉ�����N���X�͌p�����Ȃ���΂Ȃ�Ȃ��B
	*/
	class Reflectable {
	public:
		using Super = void;
		Reflectable()noexcept;
		Reflectable(const Reflectable& obj)noexcept;
		Reflectable(Reflectable&& obj)noexcept;
		virtual ~Reflectable()noexcept;
		Reflectable& operator=(const Reflectable& obj)noexcept;
		Reflectable& operator=(Reflectable&& obj)noexcept;
		//! �ϐ��܂��̓v���p�e�B��ύX����
		template<typename T>
		void SetVariable(const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief �ϐ��܂��̓v���p�e�B���擾����
		@todo �R�s�[�s�\�Ȍ^���擾�ł��Ȃ�
		*/
		template<typename T>
		void GetVariable(const std::string& var_id, T& v) {
			boost::any av;
			GetVariable_(var_id, av);
			try {
				v = boost::any_cast<T>(av);
			} catch (boost::bad_any_cast&) {
				std::string obj_tid = Reflection::GetObjectTypeIDByStdTypeInfo(typeid(*this));
				throw reflection_error(util::ConvertAndConnectToString("�N���X\"", obj_tid, "\"�̕ϐ��܂��̓v���p�e�B\"", var_id, "\"�̏������݂ɂ����Č^�̕s��v�G���[���������܂����B(�ϐ��^:", av.type().name(), ", �w��^:", typeid(T).name(), ")"));
			}
		}
		//! �N���[�����쐬����
		std::shared_ptr<Reflectable> Clone();
		//! �R�s�[����(������)
		void CopyFrom(const Reflectable& src);
		//! boost::ptree����f�[�^�̓ǂݍ���
		void LoadFromPtree(const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(const std::string& var_id, const boost::any& v);
		void GetVariable_(const std::string& var_id, boost::any& v);
	};
}

/*! ���t���N�V�����f�[�^�o�^�֐��̐錾���s���}�N��*/
#define PE_REFLECTION_DATA_REGISTERER_DECLARATION(type)\
public:\
static void ReflectionDataRegisterer(ClassRegisterer<type>& registerer);\
private:

