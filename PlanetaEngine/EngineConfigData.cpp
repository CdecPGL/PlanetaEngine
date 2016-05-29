#include "boost/lexical_cast.hpp"
#include "EngineConfigData.h"
#include "INIFileResource.h"
#include "FileSystemUtility.h"
#include "MakeResource.h"
#include "File.h"

namespace planeta_engine {
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

			namespace {
				using namespace planeta_engine::debug;
				using namespace planeta_engine::resources;
				using SectionDataType = std::pair<std::string, typename INIFileResource::SectionType>;
				bool GetSection(const INIFileResource& ini, const std::string& section_name, SectionDataType& dst) {
					auto it = ini.find(section_name);
					if (it == ini.end()) {
						PE_LOG_ERROR("�Z�N�V����\"", section_name, "\"�����݂��܂���B");
						return false;
					}
					dst = *it;
					return true;
				}

				bool SetConfigData(const SectionDataType& section, const std::string& parameter_name, bool& dst) {
					auto it = section.second.find(parameter_name);
					if (it == section.second.end()) {
						PE_LOG_ERROR("�Z�N�V����\"", section.first, "\"�Ƀp�����[�^\"", parameter_name, "\"�����݂��܂���B");
						return false;
					}
					if (it->second[0] == 't' || it->second[0] == 'T') { dst = true; } //�ꕶ���ڂ�t��������true
					else if (it->second[0] == 'f' || it->second[0] == 'F') { dst = false; } //�ꕶ���ڂ�f��������false
					else { //����ȊO�̓L���X�g���Ĕ��f
						try {
							dst = boost::lexical_cast<bool>(it->second);
						} catch (boost::bad_lexical_cast&) {
							PE_LOG_ERROR("�Z�N�V����\"", section.first, "\"�̃p�����[�^\"", parameter_name, "\"�̒l���s���ł��B");
							return false;
						}
					}
					return true;
				}

				template<typename T>
				bool SetConfigData(const SectionDataType& section, const std::string& parameter_name, T& dst) {
					auto it = section.second.find(parameter_name);
					if (it == section.second.end()) {
						PE_LOG_ERROR("�Z�N�V����\"", section.first, "\"�Ƀp�����[�^\"", parameter_name, "\"�����݂��܂���B");
						return false;
					}
					try {
						dst = boost::lexical_cast<T>(it->second);
					} catch (boost::bad_lexical_cast&) {
						PE_LOG_ERROR("�Z�N�V����\"", section.first, "\"�̃p�����[�^\"", parameter_name, "\"�̒l���s���ł��B");
						return false;
					}
					return true;
				}
			}

			bool LoadConfigData(const std::shared_ptr<File>& file) {
				assert(file != nullptr && file->GetStatus() == File::FileStatus::Available);

				auto ini_res = MakeResource<resources::INIFileResource>();
				//File����INI���\�[�X���쐬����
				if (!ini_res->Create(file)) {
					PE_LOG_ERROR("�ݒ�t�@�C����INI�t�@�C���Ƃ��ēǂݍ��ނ��Ƃ��ł��܂���ł����B");
					return false;
				}
				//�e��ݒ�f�[�^���擾����
				SectionDataType section;
				if (GetSection(*ini_res, "Game", section)
					&& SetConfigData(section, "GameTitle", game::GameTitle_)
					&& SetConfigData(section, "MajorVersion", game::MajorVersionNumber_)
					&& SetConfigData(section, "MinorVersion", game::MinorVersionNumber_)
					&& SetConfigData(section, "SubVersion", game::SubVersionNumber_)
					&& GetSection(*ini_res, "Engine", section)
					&& SetConfigData(section, "WindowMode", engine::WindowMode_)
					&& SetConfigData(section, "ColorBitDepth", engine::ColorBitDepth_)
					&& SetConfigData(section, "DrawWidth", engine::DrawSize_.x)
					&& SetConfigData(section, "DrawHeight", engine::DrawSize_.y)
					&& SetConfigData(section, "WindowWidth", engine::WindowSize_.x)
					&& SetConfigData(section, "WindowHeight", engine::WindowSize_.y)
					&& SetConfigData(section, "ResourceDecryptionKey", engine::ResourceDecryptionKey_)
					&& GetSection(*ini_res, "User", section)
					&& SetConfigData(section, "WindowSizeConfigurable", user::WindowSizeConfigurable_)
					&& SetConfigData(section, "WindowModeConfigurable", user::WindowModeConfigurable_)
					) { //�ݒ�f�[�^�擾����
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
				} else { //�ݒ�f�[�^�擾���s
					PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B");
					return false;
				}
				return true;
			}
		};
	}
}