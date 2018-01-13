#pragma once

#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

namespace plnt::reflection {
	class Reflectable;
	/*! Reflection���N���X�B���t���N�V�����ɑΉ�����N���X�͌p�����Ȃ���΂Ȃ�Ȃ��B
	*/
	class ReflectionAccessible {
	public:
		using Super = void;
		ReflectionAccessible()noexcept;
		ReflectionAccessible(const ReflectionAccessible& obj)noexcept;
		ReflectionAccessible(ReflectionAccessible&& obj)noexcept;
		virtual ~ReflectionAccessible()noexcept = 0;
		ReflectionAccessible& operator=(const ReflectionAccessible& obj)noexcept;
		ReflectionAccessible& operator=(ReflectionAccessible&& obj)noexcept;
		//! �ϐ��܂��̓v���p�e�B��ύX����
		template<typename T>
		void ReflectiveSetVariable(const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief �ϐ��܂��̓v���p�e�B���擾����
		@todo �R�s�[�s�\�Ȍ^���擾�ł��Ȃ�
		*/
		template<typename T>
		void ReflectiveGetVariable(const std::string& var_id, T& v) {
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
		std::shared_ptr<Reflectable> ReflectiveClone();
		//! �R�s�[����(������)
		void ReflectiveCopyFrom(const Reflectable& src);
		//! boost::ptree����f�[�^�̓ǂݍ���
		void ReflectiveLoadFromPtree(const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(const std::string& var_id, const boost::any& v);
		void GetVariable_(const std::string& var_id, boost::any& v);
	};
}

