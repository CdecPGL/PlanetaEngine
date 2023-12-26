#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <typeinfo>

#include "LogUtility.hpp"

namespace plnt {
	class ResourceBase;

	namespace private_ {
		class ResourceManagerInternalAccessor {
		public:
			ResourceManagerInternalAccessor(
				const std::function<std::shared_ptr<ResourceBase>(const std::type_info &, const std::string &, bool)> &
				ref_func_by_id,
				const std::function<std::shared_ptr<ResourceBase>(const std::type_info &, const std::string &,
				                                                  const std::string &, bool)> &ref_func_by_path);
			std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info &type, const std::string &id,
			                                                     bool is_valid_not_preload_warning);
			std::shared_ptr<ResourceBase> GetResourceByTypeAndPath(const std::type_info &type, const std::string &path,
			                                                       const std::string &root_path,
			                                                       bool is_valid_not_preload_warning);

		private:
			std::function<std::shared_ptr<ResourceBase>(const std::type_info &type, const std::string &, bool)>
			ref_func_by_id_;
			std::function<std::shared_ptr<ResourceBase>(const std::type_info &type, const std::string &,
			                                            const std::string &, bool)> ref_func_by_path_;
		};
	}

	class ResourceReferencer {
	public:
		ResourceReferencer(private_::ResourceManagerInternalAccessor &res_mgr_acsr, const std::string &root_path,
		                   std::vector<std::shared_ptr<ResourceBase>> &reference_list);
		/*ID�Ń��\�[�X���Q�Ƃ���B�K�v�Ȃ�V���ɓǂݍ���*/
		std::shared_ptr<ResourceBase> ReferenceResourceByTypeAndID(const std::type_info &type, const std::string &id);

		template <class RT>
		std::shared_ptr<RT> ReferenceResourceByID(const std::string &id) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = ReferenceResourceByTypeAndID(typeid(RT), id);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) { return out; } else {
					PE_LOG_ERROR("���\�[�X�̌^��ϊ��ł��܂���ł����B(\"�^�[�Q�b�g�^:", typeid(RT).name(), "\")");
					return nullptr;
				}
			} else { return nullptr; }
		}

		/*Path�Ń��\�[�X���Q�Ƃ���B�K�v�Ȃ�V���ɓǂݍ��ށB�p�X�͌��݂̃��\�[�X�p�X����ɂ������΃p�X�ƂȂ�B*/
		std::shared_ptr<ResourceBase> ReferenceResourceByTypeAndPath(const std::type_info &type,
		                                                             const std::string &path);

		template <class RT>
		std::shared_ptr<RT> ReferenceResourceByPath(const std::string &path) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = ReferenceResourceByTypeAndPath(typeid(RT), path);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) { return out; } else {
					PE_LOG_ERROR("���\�[�X�̌^��ϊ��ł��܂���ł����B(\"�^�[�Q�b�g�^:", typeid(RT).name(), "\")");
					return nullptr;
				}
			} else { return nullptr; }
		}

		/*ID����Path�Ń��\�[�X���Q�Ƃ���B�K�v�Ȃ�V���ɓǂݍ��ށB�d�������ꍇ��ID���D�悳���B�p�X�͌��݂̃��\�[�X�p�X����ɂ������΃p�X�ƂȂ�B*/
		std::shared_ptr<ResourceBase> ReferenceResourceByTypeAndIDorPath(
			const std::type_info &type, const std::string &id_or_path);

		template <class RT>
		std::shared_ptr<RT> ReferenceResourceByIDorPath(const std::string &id_or_path) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = ReferenceResourceByTypeAndIDorPath(typeid(RT), id_or_path);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) { return out; } else {
					PE_LOG_ERROR("���\�[�X�̌^��ϊ��ł��܂���ł����B(\"�^�[�Q�b�g�^:", typeid(RT).name(), "\")");
					return nullptr;
				}
			} else { return nullptr; }
		}

	private:
		std::vector<std::shared_ptr<ResourceBase>> &reference_list_;
		private_::ResourceManagerInternalAccessor &manager_accessor_;
		std::string root_path_;
	};
}
