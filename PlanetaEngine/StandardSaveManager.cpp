#include "boost/lexical_cast.hpp"
#include "StandardSaveManager.h"
#include "DataContainer.h"
#include "DataContainerSerializeUtility.h"
#include "FileManipulator.h"
#include "LogUtility.h"
#include "RCsv.h"

namespace {
	constexpr char* CommonSaveDataFileName("common_save_data");
	constexpr char* UserSaveDataFileName("user_save_data");
	constexpr char* SaveDataInformationFileName("save_data_info");
}

namespace planeta {
	namespace private_ {
		//////////////////////////////////////////////////////////////////////////
		//Impl_
		//////////////////////////////////////////////////////////////////////////
		class StandardSaveManager::Impl_ {
		public:
			std::unique_ptr<util::DataContainer> common_data_;
			std::unique_ptr<util::DataContainer> current_user_data_;
			std::shared_ptr<FileManipulator> file_accessor_;
			int current_user_data_idx_ = -1; //-1で読み込んでいない

			struct SaveDataInfo {
				int user_save_data_count = 0;
				int user_save_data_param_count = 0;
				std::vector<std::pair<int,UserDataHeader>> user_data_info; //<番号,ユーザーデータ>
			};

			SaveDataInfo save_data_info_;

			bool LoadSaveDataInformation() {
				//セーブデータ情報の読み込み
				auto file = file_accessor_->LoadFile(SaveDataInformationFileName);
				if (file == nullptr) {
					PE_LOG_ERROR("セーブデータ情報ファイルの取得に失敗しました。");
					return false;
				}
				//実装中

				return true;
			}
			bool LoadCommonSaveData() {
				auto file = file_accessor_->LoadFile(CommonSaveDataFileName);
				if (file == nullptr) {
					PE_LOG_ERROR("共通セーブデータファイルの取得に失敗しました。");
					return false;
				}
				try {
					common_data_ = std::make_unique<util::DataContainer>(std::move(util::DeserializeDataContainer(file)));
				} catch (std::runtime_error&) {
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
				std::string file_name = std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(save_data_info_.user_data_info[idx].first);
				auto file = file_accessor_->LoadFile(file_name);
				if (file == nullptr) {
					PE_LOG_ERROR("ユーザーセーブデータ", idx, "ファイルの取得に失敗しました。");
					return false;
				}
				try {
					current_user_data_ = std::make_unique<util::DataContainer>(std::move(util::DeserializeDataContainer(file)));
				} catch (std::runtime_error&) {
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
				auto file = util::SerializeDataContainer(*current_user_data_);
				if (file == nullptr) {
					PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx_, "のファイル作成に失敗しました。");
					return false;
				}
				if (!file_accessor_->SaveFile(std::string(UserSaveDataFileName) + boost::lexical_cast<std::string>(current_user_data_idx_), std::move(*file))) {
					PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx_, "の保存に失敗しました。");
					return false;
				}
				return true;
			}
			bool SaveCommonSaveData() {
				auto file = util::SerializeDataContainer(*common_data_);
				if (file == nullptr) {
					PE_LOG_ERROR("共通セーブデータのファイル作成に失敗しました。");
					return false;
				}
				if (!file_accessor_->SaveFile(CommonSaveDataFileName, std::move(*file))) {
					PE_LOG_ERROR("共通セーブデータの保存に失敗しました。");
					return false;
				}
				return true;
			}
			bool SaveCurrentUserSaveData() {
				

			}
		};


		//////////////////////////////////////////////////////////////////////////
		//SaveDataManager
		//////////////////////////////////////////////////////////////////////////
		StandardSaveManager::StandardSaveManager() :impl_(std::make_unique<Impl_>()) {}

		StandardSaveManager::~StandardSaveManager() = default;

		bool StandardSaveManager::Initialize() {
			assert(impl_->file_accessor_ != nullptr);
			//セーブデータ情報の読み込み
			//if (!impl_->LoadSaveDataInformation()) { return false; }
			//共通セーブデータのロード
			//if (!impl_->LoadCommonSaveData()) { return false; }

			//PE_LOG_MESSAGE("セーブデータを正常に読み込みました。");
			PE_LOG_WARNING("SaveDataManagerは未実装です。");
			return true;
		}

		void StandardSaveManager::Finalize() {
			//Save();
		}

		void StandardSaveManager::SetFileManipurator_(const std::shared_ptr<FileManipulator>& file_accessor) {
			impl_->file_accessor_ = file_accessor;
		}

		void StandardSaveManager::Save() {
			//共通データ
			impl_->SaveCommonSaveData();
			//ユーザーデータ

			//セーブデータリスト更新
			impl_->SaveSaveDataInformation();
		}

		bool StandardSaveManager::LoadUserData(int idx) {
			return impl_->LoadUserSaveData(idx);
		}

		int StandardSaveManager::GetUserDataConut() const {
			return impl_->save_data_info_.user_save_data_count;
		}

		int StandardSaveManager::GetUserDataHeaderParamCount() const {
			return impl_->save_data_info_.user_save_data_param_count;
		}

		const util::DataContainer& StandardSaveManager::GetCommonData() const {
			assert(impl_->common_data_ != nullptr);
			return  *impl_->common_data_;
		}

		util::DataContainer& StandardSaveManager::GetCommonData() {
			assert(impl_->common_data_ != nullptr);
			return  *impl_->common_data_;
		}

		boost::optional<const util::DataContainer&> StandardSaveManager::GetCurrentData() const {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<const util::DataContainer&>(*impl_->common_data_);
			} else {
				return boost::optional<const util::DataContainer&>();
			}
		}

		boost::optional<util::DataContainer&> StandardSaveManager::GetCurrentData() {
			if (impl_->current_user_data_idx_ >= 0) {
				assert(impl_->current_user_data_ != nullptr);
				return boost::optional<util::DataContainer&>(*impl_->common_data_);
			} else {
				return boost::optional<util::DataContainer&>();
			}
		}

	}
}