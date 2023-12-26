#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include "LogUtility.hpp"

namespace plnt {
	class ResourceBase;

	/// <summary>���\�[�X�}�l�[�W�����J�C���^�[�t�F�C�X�B</summary>
	class IResourceManager {
	public:
		/// <summary>�f�X�g���N�^�B</summary>
		virtual ~IResourceManager() = default;
		/// <summary>���\�[�X��ID�Ŏ擾����B</summary>
		/// <param name="type_info">�擾���郊�\�[�X�̌^���</param>
		/// <param name="id">�擾���郊�\�[�X�̃��\�[�XID</param>
		/// <returns>�擾���ꂽ���\�[�X�B�擾�ł��Ȃ������ꍇ��nullptr</returns>
		virtual std::shared_ptr<ResourceBase> GetResourceByTypeAndID(const std::type_info &type_info,
		                                                             const std::string &id) = 0;
		/// <summary>���\�[�X��ID�Ŏ擾���^��ϊ�����B</summary>
		/// <param name="id">�擾���郊�\�[�X�̃��\�[�XID</param>
		/// <returns>�擾���ꂽ���\�[�X�B�擾�ł��Ȃ������ꍇ��nullptr</returns>
		template <class RT>
		std::shared_ptr<RT> GetResourceByID(const std::string &id) {
			static_assert(std::is_base_of<ResourceBase, RT>::value, "RT must derive ResourceBase");
			auto rsc = GetResourceByTypeAndID(typeid(RT), id);
			if (rsc) {
				auto out = std::dynamic_pointer_cast<RT>(rsc);
				if (out) { return out; } else {
					PE_LOG_ERROR("���\�[�X�̌^��ϊ��ł��܂���ł����B(\"�^�[�Q�b�g�^:", typeid(RT).name(), "\")");
					return nullptr;
				}
			} else { return nullptr; }
		}

		/// <summary>���\�[�X��FullID�Ŏ擾����B</summary>
		/// <param name="full_id">�擾���郊�\�[�X�̃��\�[�XFullID</param>
		/// <returns>�擾���ꂽ���\�[�X�B�擾�ł��Ȃ������ꍇ��nullptr</returns>
		virtual std::shared_ptr<ResourceBase> GetResourceByFullID(const std::string &full_id) = 0;
	};
}
