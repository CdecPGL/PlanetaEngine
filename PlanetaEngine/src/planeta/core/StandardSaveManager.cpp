#include "boost/lexical_cast.hpp"
#include "StandardSaveManager.hpp"
#include "data_container.hpp"
#include "data_container_serialize_utility.hpp"
#include "file_manipulator.hpp"
#include "log_utility.hpp"
#include "planeta/buildin/RCsv.hpp"

namespace {
	constexpr char *CommonSaveDataFileName(const_cast<char *>("common_save_data"));
	constexpr char *UserSaveDataFileName(const_cast<char *>("user_save_data"));
	constexpr char *SaveDataInformationFileName(const_cast<char *>("save_data_info"));
}

namespace plnt {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////
		class StandardSaveManager::Impl_ {
		public:
			std::unique_ptr<util::data_container> common_data_;
			std::unique_ptr<util::data_container> current_user_data_;
			std::shared_ptr<file_manipulator> file_accessor_;
			int current_user_data_idx_ = -1; //-1で読み込んでいない

			struct SaveDataInfo {
				int user_save_data_count = 0;
				int user_save_data_param_count = 0;
				std::vector<std::pair<int, UserDataHeader>> user_data_info; //<番号,ユーザーデータ>
			};

			SaveDataInfo save_data_info_;

			bool LoadSaveDataInformation() {
				//セーブデータ情報の読み込み
				auto file = file_accessor_->load_file(SaveDataInformationFileName);
				if (file == nullptr) {
					PE_LOG_ERROR("セーブデータ情報ファイルの取得に失敗しました。");
					return false;
				}
				//実装中

				return true;
			}

			bool LoadCommonSaveData() {
				auto file = file_accessor_->load_file(CommonSaveDataFileName);
				if (file == nullptr) {
					PE_LOG_ERROR("共通セーブデータファイルの取得に失敗しました。");
					return false;
				}
				try {
					common_data_ = std::make_unique<util::data_container>(
						std::move(util::deserialize_data_container(file)));
				} catch (std::runtime_error &) {
					PE_LOG_ERROR("共通セーブデータの読み込みに失敗しました。");
					return false;
				}
				return true;
			}

			bool LoadUserSaveData(int idx) {
				if (idx < 0 || save_data_info_.user_save_data_count <= idx) {
					PE_LOG_ERROR("範囲外のユーザーセーブデータ番号が指定されました。(", idx, ")");
					return false;
				}
				std::string file_name = std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(
					save_data_info_.user_data_info[idx].first);
				auto file = file_accessor_->load_file(file_name);
				if (file == nullptr) {
					PE_LOG_ERROR("ユーザーセーブデータ", idx, "ファイルの取得に失敗しました。");
					return false;
				}
				try {
					current_user_data_ = std::make_unique<util::data_container>(
						std::move(util::deserialize_data_container(file)));
				} catch (std::runtime_error &) {
					PE_LOG_ERROR("ユーザーセーブデータ", idx, "の読み込みに失敗しました。");
					return false;
				}
				return true;
			}

			bool SaveSaveDataInformation() {
				if (current_user_data_ < 0 || save_data_info_.user_save_data_count <= current_user_data_idx_) {
					PE_LOG_ERROR("範囲外のユーザーセーブデータ番号が指定されました。(", current_user_data_idx_, ")");
					return false;
				}
				assert(current_user_data_ != nullptr);
				auto file = util::serialize_data_container(*current_user_data_);
				if (file == nullptr) {
					PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx_, "のファイル作成に失敗しました。");
					return false;
				}
				if (!file_accessor_->save_file(
					std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(current_user_data_idx_),
					std::move(*file))) {
					PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx_, "の保存に失敗しました。");
					return false;
				}
				return true;
			}

			bool SaveCommonSaveData() {
				auto file = util::serialize_data_container(*common_data_);
				if (file == nullptr) {
					PE_LOG_ERROR("共通セーブデータのファイル作成に失敗しました。");
					return false;
				}
				if (!file_accessor_->save_file(CommonSaveDataFileName, std::move(*file))) {
					PE_LOG_ERROR("共通セーブデータの保存に失敗しました。");
					return false;
				}
				return true;
			}

			bool SaveCurrentUserSaveData() { }
		};


		//////////////////////////////////////////////////////////////////////////
		//SaveDataManager
		//////////////////////////////////////////////////////////////////////////
		StandardSaveManager::StandardSaveManager() : impl_(std::make_unique<Impl_>()) { }

		StandardSaveManager::~StandardSaveManager() = default;

		bool StandardSaveManager::initialize() {
			assert(impl_->file_accessor_ != nullptr);
			//セーブデータ情報の読み込み
			//if (!impl_->LoadSaveDataInformation()) { return false; }
			//共通セーブデータのロード
			//if (!impl_->LoadCommonSaveData()) { return false; }

			//PE_LOG_MESSAGE("セーブデータを正常に読み込みました。");
			PE_LOG_WARNING("SaveDataManagerは未実装です。");
			return true;
		}

		void StandardSaveManager::finalize() {
			//Save();
		}

		void StandardSaveManager::set_file_manipulator(const std::shared_ptr<file_manipulator> &file_accessor) {
			impl_->file_accessor_ = file_accessor;
		}

		void StandardSaveManager::Save() {
			//共通データ
			impl_->SaveCommonSaveData();
			//ユーザーデータ

			//セーブデータリスト更新
			impl_->SaveSaveDataInformation();
		}

		bool StandardSaveManager::LoadUserData(int idx) { return impl_->LoadUserSaveData(idx); }

		int StandardSaveManager::GetUserDataConut() const { return impl_->save_data_info_.user_save_data_count; }

		int StandardSaveManager::GetUserDataHeaderParamCount() const {
			return impl_->save_data_info_.user_save_data_param_count;
		}

		const util::data_container &StandardSaveManager::GetCommonData() const {
			assert(impl_->common_data_ != nullptr);
			return *impl_->common_data_;
		}

		util::data_container &StandardSaveManager::GetCommonData() {
			assert(impl_->common_data_ != nullptr);
			return *impl_->common_data_;
		}

		boost::optional<const util::data_container &> StandardSaveManager::GetCurrentData() const {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<const util::data_container &>(*impl_->common_data_);
			} else { return boost::optional<const util::data_container &>(); }
		}

		boost::optional<util::data_container &> StandardSaveManager::GetCurrentData() {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<util::data_container &>(*impl_->common_data_);
			} else { return boost::optional<util::data_container &>(); }
		}
	}
}
