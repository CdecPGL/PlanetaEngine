#include "boost/lexical_cast.hpp"
#include "EngineConfigData.h"
#include "RJson.h"
#include "FileSystemUtility.h"
#include "RIni.h"
#include "MakeResource.h"
#include "File.h"

namespace planeta {
	namespace core {
		namespace engine_config {
			std::string game::GameTitle_("NULL"); //�Q�[���^�C�g��
			std::string game::VersionString_("0.0.0"); //�o�[�W����������
			int game::MajorVersionNumber_(0);
			int game::MinorVersionNumber_(0);
			int game::SubVersionNumber_(0);

			bool engine::WindowMode_(true); //�E�C���h�E���[�h��
			int engine::ColorBitDepth_(32); //�J���[�r�b�g�[�x
			Vector2Di engine::DrawSize_(640, 480); //�`��U�C�Y
			Vector2Di engine::WindowSize_(640, 480); //�E�C���h�E�T�C�Y
			unsigned int engine::ResourceDecryptionKey_(0);

			bool user::WindowSizeConfigurable_(false); //�E�C���h�E�T�C�Y�����[�U�[���ݒ�\��
			bool user::WindowModeConfigurable_(false); //�E�C���h�E���[�h���ۂ������[�U�[���ݒ�\��

			bool LoadConfigData(const std::shared_ptr<File>& file) {
				assert(file != nullptr && file->GetStatus() == File::FileStatus::Available);

				auto json_res = MakeResource<RJson>();
				//File����JSON���\�[�X���쐬����
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("�ݒ�t�@�C����JSON�t�@�C���Ƃ��ēǂݍ��ނ��Ƃ��ł��܂���ł����B");
					return false;
				}
				try {
					decltype(auto) root_obj = json_res->GetRoot().GetWithException<JSONObject>();
					auto game_obj = root_obj->AtWithException("Game")->GetWithException<JSONObject>();
					auto engine_obj = root_obj->AtWithException("Engine")->GetWithException<JSONObject>();
					auto user_obj = root_obj->AtWithException("User")->GetWithException<JSONObject>();
					game::GameTitle_ = *game_obj->AtWithException("Title")->GetWithException<std::string>();
					auto buf_array = *game_obj->AtWithException("Version")->GetWithException<std::vector<int>>();
					game::MajorVersionNumber_ = buf_array[0];
					game::MinorVersionNumber_ = buf_array[1];
					game::SubVersionNumber_ = buf_array[2];
					engine::WindowMode_ = *engine_obj->AtWithException("WindowMode")->GetWithException<bool>();
					engine::ColorBitDepth_ = *engine_obj->AtWithException("ColorBitDepth")->GetWithException<int>();
					buf_array = *engine_obj->AtWithException("DrawSize")->GetWithException<std::vector<int>>();
					engine::DrawSize_.Set(buf_array[0], buf_array[1]);
					buf_array = *engine_obj->AtWithException("WindowSize")->GetWithException<std::vector<int>>();
					engine::WindowSize_.Set(buf_array[0], buf_array[1]);
					engine::ResourceDecryptionKey_ = *engine_obj->AtWithException("ColorBitDepthResourceDecryptionKey")->GetWithException<int>();
					user::WindowSizeConfigurable_ = *user_obj->AtWithException("WindowSizeConfigurable")->GetWithException<bool>();
					user::WindowModeConfigurable_ = *user_obj->AtWithException("WindowSModeConfigurable")->GetWithException<bool>();
				} catch (std::out_of_range& e) {
					PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B���e���s�����Ă���\��������܂��B(", e.what(), ")");
					return false;
				} catch (JSONTypeError& e) {
					PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B�^���ԈႦ�Ă���\��������܂��B(", e.what(), ")");
					return false;
				}
				//�ݒ�f�[�^�擾����
				//�o�[�W����������ݒ�
				game::VersionString_ =
					boost::lexical_cast<std::string>(game::MajorVersionNumber())
					+ "." + boost::lexical_cast<std::string>(game::MinorVersionNumber())
					+ "." + boost::lexical_cast<std::string>(game::SubVersionNumber());
				//�ݒ�����O�ɏo��
				using namespace debug;
				auto& sys_log = SystemLog::instance();
				PE_LOG_MESSAGE("�G���W���ݒ��ǂݍ��݂܂����B");
				sys_log.SimpleLog("--------�G���W���ݒ���--------");
				sys_log.SimpleLog("�Q�[���^�C�g�� : ", game::GameTitle());
				sys_log.SimpleLog("�Q�[���o�[�W���� : ", game::VersionString());
				sys_log.SimpleLog("�E�C���h�E���[�h : ", engine::WindowMode() ? "�L��" : "����");
				sys_log.SimpleLog("�J���[�r�b�g�[�x : ", engine::ColorBitDepth());
				sys_log.SimpleLog("�`��(����)�T�C�Y : ", engine::DrawSize().ToString());
				sys_log.SimpleLog("�E�C���h�E(�\��)�T�C�Y : ", engine::WindowSize().ToString());
				sys_log.SimpleLog("���[�U�[�ɂ��E�C���h�E(�\��)�T�C�Y�̐ݒ� : ", user::WindowSizeConfigurable() ? "�L��" : "����");
				sys_log.SimpleLog("���[�U�[�ɂ��E�C���h�E���[�h�̐ݒ� : ", user::WindowModeConfigurable() ? "�L��" : "����");
				sys_log.SimpleLog("--------------------------------");
				return true;
			}
		};
	}
}