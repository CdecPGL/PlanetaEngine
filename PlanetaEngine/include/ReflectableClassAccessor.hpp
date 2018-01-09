#pragma once

#include <memory>
#include <string>

#include "boost/property_tree/ptree.hpp"

namespace plnt {
	class Reflectable;
	namespace private_ {
		struct ClassInfo;
	}
	class ReflectableClassAccessor final {
	public:
		using Super = void;
		ReflectableClassAccessor(const private_::ClassInfo* ci)noexcept;
		ReflectableClassAccessor(const ReflectableClassAccessor& obj) = delete;
		ReflectableClassAccessor(ReflectableClassAccessor&& obj) = delete;
		virtual ~ReflectableClassAccessor()noexcept;
		ReflectableClassAccessor& operator=(const ReflectableClassAccessor& obj) = delete;
		ReflectableClassAccessor& operator=(ReflectableClassAccessor&& obj) = delete;
		//! �ϐ��܂��̓v���p�e�B��ύX����
		template<typename T>
		void SetVariable(Reflectable& obj, const std::string& var_id, const T& v) {
			SetVariable_(var_id, v);
		}
		/*!
		@brief �ϐ��܂��̓v���p�e�B���擾����
		@todo �R�s�[�s�\�Ȍ^���擾�ł��Ȃ�
		*/
		template<typename T>
		void GetVariable(Reflectable& obj, const std::string& var_id, T& v) {
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
		std::shared_ptr<Reflectable> Clone(Reflectable& obj);
		//! �R�s�[����(������)
		void Copy(Reflectable& dst, const Reflectable& src);
		//! boost::ptree����f�[�^�̓ǂݍ���
		void LoadFromPtree(Reflectable& obj, const boost::property_tree::ptree& pt);
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
		void SetVariable_(Reflectable& obj, const std::string& var_id, const boost::any& v);
		void GetVariable_(Reflectable& obj, const std::string& var_id, boost::any& v);
	};
}