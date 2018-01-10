#pragma once

#include "planeta/Game.hpp"
#include "planeta/IResourceManager.hpp"
#include "planeta/LogUtility.hpp"

namespace plnt {
	namespace util {
		/*! ���\�[�X��ێ����邽�߂̃w���p�[�N���X*/
		template<class ResType>
		class ResourceHolder final{
		public:
			/*! �R���X�g���N�^*/
			ResourceHolder() = default;
			/*! �f�X�g���N�^*/
			~ResourceHolder() = default;
			/*! �R�s�[�R���X�g���N�^*/
			ResourceHolder(const ResourceHolder& obj) = default;
			/*! ���[�u�R���X�g���N�^*/
			ResourceHolder(ResourceHolder&& obj) = default;
			/*! ������Z�q*/
			ResourceHolder& operator=(const ResourceHolder& obj) = default;
			/*! ���[�u�R���X�g���N�^*/
			ResourceHolder& operator=(ResourceHolder&& obj) = default;
			/*! ID�Ń��\�[�X��ݒ肷��*/
			bool SetResourceByID(const std::string& res_id)noexcept {
				auto res = Game::instance().resource_manager()->GetResourceByTypeAndID(typeid(ResType), res_id);
				if (res == nullptr) {
					PE_LOG_ERROR("���\�[�X�̎擾�Ɏ��s���܂����B(���\�[�XID:", res_id, ")");
					return false;
				}
				std::shared_ptr<ResType> gr = std::dynamic_pointer_cast<ResType>(res);
				if (gr) {
					return true;
				} else {
					PE_LOG_ERROR("���\�[�X�̌^�ϊ��Ɏ��s���܂����B(���\�[�XID:", res_id, "�A�\�[�X�̌^", typeid(*res).name(), "�A�^�[�Q�b�g�̌^:", typeid(ResType).name(), ")");
					return false;
				}
			}
			/*! ���\�[�X�̃|�C���^���擾����*/
			std::shared_ptr<ResType> resource_shared_pointer()const noexcept {
				if (!is_available()) {
					PE_LOG_ERROR("���\�[�X�����p�s�\�ł��B");
					return nullptr;
				}
				return resource_;
			}
			/*! ���\�[�X�̎Q�Ƃ��擾����
				@exception ���\�[�X�����p�s�\�ȏꍇ��std::runtime_error�𓊂���B
			*/
			ResType& resource_reference()const {
				if (!is_available()) {
					PE_LOG_ERROR("���\�[�X�����p�s�\�ł��B");
					throw std::runtime_error("���\�[�X�����p�s�\�ł��B");
				}
				return *resource_;
			}
			/*! ���\�[�X�����p�\���ǂ������擾����*/
			bool is_available()const noexcept {
				return resource_ != nullptr;
			}
		private:
			std::shared_ptr<ResType> resource_;
		};
	}
}