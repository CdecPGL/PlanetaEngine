#include "planeta/StandardConfigManager.hpp"
#include "planeta/JsonFile.hpp"
#include "planeta/LogUtility.hpp"
#include "planeta/StringUtility.hpp"

namespace plnt {
	namespace private_ {
		bool StandardConfigManager::LoadSystemConfig(const File& file) {
			JsonFile json_file{};
			//File����JSON���\�[�X���쐬����
			if (!json_file.Load(file)) {
				PE_LOG_ERROR("�V�X�e���ݒ�t�@�C����JSON�t�@�C���Ƃ��ēǂݍ��ނ��Ƃ��ł��܂���ł����B");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.GetRoot().GetWithException<JSONObject>();
				//Game
				auto game_obj = root_obj->AtWithException("Game")->GetWithException<JSONObject>();
				game_title_ = *game_obj->AtWithException("Title")->GetWithException<std::string>();
				auto buf_array = *game_obj->AtWithException("Version")->GetWithException<std::vector<int>>();
				game_version_numbers_[0] = buf_array[0];
				game_version_numbers_[1] = buf_array[1];
				game_version_numbers_[2] = buf_array[2];
				//Engine
				auto engine_obj = root_obj->AtWithException("Engine")->GetWithException<JSONObject>();
				color_bit_depth_ = *engine_obj->AtWithException("ColorBitDepth")->GetWithException<int>();
				buf_array = *engine_obj->AtWithException("DrawSize")->GetWithException<std::vector<int>>();
				draw_size_.Set(buf_array[0], buf_array[1]);
				is_cursor_visible_ = *engine_obj->AtWithException("IsCursorVisible")->GetWithException<bool>();
				//Program
				auto prog_obj = root_obj->AtWithException("Program")->GetWithException<JSONObject>();
				auto scene_obj = prog_obj->AtWithException("Scene")->GetWithException<JSONObject>();
				//-�V�[��
				startup_scene_id_ = *scene_obj->AtWithException("Startup")->GetWithException<std::string>();
				//-�Փ˃V�X�e��
				auto col_obj = prog_obj->AtWithException("Collision")->GetWithException<JSONObject>();
				//--�Փ˃O���[�v
				std::vector<std::string> group_list;
				group_list = *col_obj->AtWithException("Groups")->GetWithException<std::vector<std::string>>();
				collision_group_matrix_.AddCollisionGroups(group_list);
				//--�Փˉ\�}�g���b�N�X
				auto col_mtx = *col_obj->AtWithException("CollidableMatrix")->GetWithException<std::unordered_map<std::string, std::vector<std::string>>>();
				for (auto&& group : col_mtx) {
					for (auto&& cbl_group : group.second) {
						if (!collision_group_matrix_.SetCollisionFlag(group.first, cbl_group, true)) {
							PE_LOG_WARNING("�Փ˃O���[�v\"", group.first, "\"��\"", cbl_group, "\"�̃t���O�̐ݒ�Ɏ��s���܂����B�ǂ��炩�̃O���[�v����`����Ă��Ȃ��\��������܂��B");
						}
					}
				}
			} catch (std::out_of_range& e) {
				PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B���e���s�����Ă���\��������܂��B(", e.what(), ")");
				return false;
			} catch (JSONTypeError& e) {
				PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B�^���ԈႦ�Ă���\��������܂��B(", e.what(), ")");
				return false;
			}
			//�ݒ�f�[�^�擾����
			//�o�[�W����������ݒ�
			game_version_string_ = util::ConvertAndConnectToString(game_version_numbers_[0], ".", game_version_numbers_[1], ".", game_version_numbers_[2]);
			//�ݒ�����O�ɏo��
			auto& sys_log = *Game::instance().log_manager();
			PE_LOG_MESSAGE("�V�X�e���ݒ��ǂݍ��݂܂����B");
			sys_log.SimpleLog("--------�V�X�e���ݒ���--------");
			sys_log.SimpleLog("�Q�[���^�C�g�� : ", game_title_);
			sys_log.SimpleLog("�Q�[���o�[�W���� : ", game_version_string_);
			sys_log.SimpleLog("�J���[�r�b�g�[�x : ", color_bit_depth_);
			sys_log.SimpleLog("�`��(����)�T�C�Y : ", draw_size_.ToString());
			sys_log.SimpleLog("--------------------------------");
			return true;
		}

		bool StandardConfigManager::LoadUserConfig(const File& file) {
			JsonFile json_file{};
			//File����JSON���\�[�X���쐬����
			if (!json_file.Load(file)) {
				PE_LOG_ERROR("���[�U�[�ݒ�t�@�C����JSON�t�@�C���Ƃ��ēǂݍ��ނ��Ƃ��ł��܂���ł����B");
				return false;
			}
			try {
				decltype(auto) root_obj = json_file.GetRoot().GetWithException<JSONObject>();
				//Window
				auto window_obj = root_obj->AtWithException("Window")->GetWithException<JSONObject>();
				auto buf_array = *window_obj->AtWithException("WindowSize")->GetWithException<std::vector<int>>();
				window_size_.Set(buf_array[0], buf_array[1]);
				is_window_mode_ = *window_obj->AtWithException("WindowMode")->GetWithException<bool>();
			} catch (std::out_of_range& e) {
				PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B���e���s�����Ă���\��������܂��B(", e.what(), ")");
				return false;
			} catch (JSONTypeError& e) {
				PE_LOG_ERROR("�ݒ�t�@�C������f�[�^���擾���邱�Ƃ��ł��܂���ł����B�^���ԈႦ�Ă���\��������܂��B(", e.what(), ")");
				return false;
			}
			//�ݒ�f�[�^�擾����
			//�ݒ�����O�ɏo��
			auto& sys_log = *Game::instance().log_manager();
			PE_LOG_MESSAGE("���[�U�[�ݒ��ǂݍ��݂܂����B");
			sys_log.SimpleLog("--------���[�U�[�ݒ���--------");
			sys_log.SimpleLog("�E�C���h�E���[�h : ", is_window_mode_ ? "�L��" : "����");
			sys_log.SimpleLog("�E�C���h�E(�\��)�T�C�Y : ", window_size_.ToString());
			sys_log.SimpleLog("--------------------------------");
			return true;
		}

		int StandardConfigManager::color_bit_depth() const {
			return color_bit_depth_;
		}

		std::string StandardConfigManager::startup_scene_id() const {
			return startup_scene_id_;
		}

		std::string StandardConfigManager::game_title() const {
			return game_title_;
		}

		std::string StandardConfigManager::game_version_string() const {
			return game_version_string_;
		}

		std::array<int, 3> StandardConfigManager::game_version_numbers() const {
			return game_version_numbers_;
		}

		bool StandardConfigManager::is_window_mode() const {
			return is_window_mode_;
		}

		plnt::Vector2Di StandardConfigManager::draw_size() const {
			return draw_size_;
		}

		plnt::Vector2Di StandardConfigManager::window_size() const {
			return window_size_;
		}

		const plnt::CollisionGroupMatrix& StandardConfigManager::collision_group_matrix() const {
			return collision_group_matrix_;
		}

		bool StandardConfigManager::is_cursor_visible() const {
			return is_cursor_visible_;
		}

	}
}