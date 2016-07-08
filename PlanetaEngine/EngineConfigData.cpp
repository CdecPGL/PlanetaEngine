#include "boost/lexical_cast.hpp"
#include "EngineConfigData.h"
#include "JSONResource.h"
#include "FileSystemUtility.h"
#include "INIFileResource.h"
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

			namespace {
				using namespace planeta::debug;
				using namespace planeta::resources;
				template<typename T>
				bool SetValueFromJSONObject(const JSONObject& json_obj, const std::string& key, T& dst) {
					//bool�ȊO�̐��l�^��double�Ƃ��Ĉ���
					using TT = std::conditional_t<std::is_same<T, bool>::value, bool,
						std::conditional_t<std::is_arithmetic<T>::value, double, T>>;
					decltype(auto) val = json_obj.At<TT>(key);
					if (val) {
						dst = static_cast<T>(*val);
						return true;
					} else {
						PE_LOG_ERROR("�L�[\"", key, "\"�̒l���擾�ł��܂���ł����B");
						return false;
					}
				}
				template<typename T>
				bool SetCRefOptFromJSONObject(const JSONObject& json_obj, const std::string& key,  boost::optional<const T&>& dst) {
					decltype(auto) val = json_obj.At<T>(key);
					if (val) {
						dst = val;
						return true;
					} else {
						PE_LOG_ERROR("�L�[\"", key, "\"�̒l���擾�ł��܂���ł����B");
						return false;
					}
				}
				bool SetVector2DiFromJSONArray(const JSONArray& json_ary,Vector2Di& dst) {
					decltype(auto) opt = json_ary.At<double>(0);
					if (opt) {
						dst.x = static_cast<int>(*opt);
					} else {
						PE_LOG_ERROR("JSONArray[0]��Vector2Di.x�ɕϊ��ł��܂���ł����B");
						return false;
					}
					opt = json_ary.At<double>(1);
					if (opt) {
						dst.y = static_cast<int>(*opt);
					} else {
						PE_LOG_ERROR("JSONArray[1]��Vector2Di.y�ɕϊ��ł��܂���ł����B");
						return false;
					}
					return true;
				}
				template<typename T,int N>
				bool SetDataToPointerArrayFromJSONArray(const JSONArray& json_ary, const std::array<T*, N>& ary) {
					//bool�ȊO�̐��l�^��double�Ƃ��Ĉ���
					using TT = std::conditional_t<std::is_same<T, bool>::value, bool,
						std::conditional_t<std::is_arithmetic<T>::value, double, T>>;
					for (int i = 0; i < N; ++i) {
						decltype(auto) opt = json_ary.At<TT>(i);
						if (opt) {
							*ary[i] = static_cast<T>(*opt);
						} else {
							PE_LOG_ERROR("JSONArray[", i, "]��std::array<N,T*>[", i, "]�ɃZ�b�g�ł��܂���ł����B");
							return false;
						}
					}
					return false;
				}
			}

			bool LoadConfigData(const std::shared_ptr<File>& file) {
				assert(file != nullptr && file->GetStatus() == File::FileStatus::Available);

				auto json_res = MakeResource<resources::JSONResource>();
				//File����INI���\�[�X���쐬����
				if (!json_res->Create(file)) {
					PE_LOG_ERROR("�ݒ�t�@�C����JSON�t�@�C���Ƃ��ēǂݍ��ނ��Ƃ��ł��܂���ł����B");
					return false;
				}
				decltype(auto) root_obj = json_res->GetRoot().Get<JSONObject>();
				boost::optional<const JSONObject&> game_obj;
				boost::optional<const JSONObject&> engine_obj;
				boost::optional<const JSONObject&> user_obj;
				boost::optional<const JSONArray&> buffer_array;
				if (SetCRefOptFromJSONObject(*root_obj, "Game", game_obj)
					&& SetValueFromJSONObject(*game_obj, "Title", game::GameTitle_)
					&& SetCRefOptFromJSONObject(*game_obj, "Version", buffer_array)
					&& SetDataToPointerArrayFromJSONArray(*buffer_array, std::array<int*,3>{&game::MajorVersionNumber_,&game::MinorVersionNumber_,&game::SubVersionNumber_})
					&& SetCRefOptFromJSONObject(*root_obj, "Engine", engine_obj)
					&& SetValueFromJSONObject(*engine_obj, "WindowMode", engine::WindowMode_)
					&& SetValueFromJSONObject(*engine_obj, "ColorBitDepth", engine::ColorBitDepth_)
					&& SetCRefOptFromJSONObject(*engine_obj, "DrawSize", buffer_array)
					&& SetVector2DiFromJSONArray(*buffer_array,engine::DrawSize_)
					&& SetCRefOptFromJSONObject(*engine_obj, "WindowSize", buffer_array)
					&& SetVector2DiFromJSONArray(*buffer_array, engine::WindowSize_)
					&& SetValueFromJSONObject(*engine_obj, "ResourceDecryptionKey", engine::ResourceDecryptionKey_)
					&& SetCRefOptFromJSONObject(*root_obj, "User", user_obj)
					&& SetValueFromJSONObject(*user_obj, "WindowSizeConfigurable", user::WindowSizeConfigurable_)
					&& SetValueFromJSONObject(*user_obj, "WindowModeConfigurable", user::WindowModeConfigurable_)
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