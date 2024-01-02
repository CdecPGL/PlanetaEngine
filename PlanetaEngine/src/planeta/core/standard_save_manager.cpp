#include "boost/lexical_cast.hpp"

#include "standard_save_manager.hpp"
#include "data_container.hpp"
#include "data_container_serialize_utility.hpp"
#include "file_manipulator.hpp"
#include "log_utility.hpp"
#include "planeta/buildin/RCsv.hpp"

namespace {
	constexpr char *common_save_data_file_name(const_cast<char *>("common_save_data"));
	constexpr char *user_save_data_file_name(const_cast<char *>("user_save_data"));
	constexpr char *save_data_information_file_name(const_cast<char *>("save_data_info"));
}

namespace plnt::private_ {
	//////////////////////////////////////////////////////////////////////////
	//Impl_
	//////////////////////////////////////////////////////////////////////////
	class standard_save_manager::impl {
	public:
		std::unique_ptr<util::data_container> common_data;
		std::unique_ptr<util::data_container> current_user_data;
		std::shared_ptr<file_manipulator> file_accessor;
		int current_user_data_idx = -1; //-1で読み込んでいない

		struct save_data_info {
			int user_save_data_count = 0;
			int user_save_data_param_count = 0;
			std::vector<std::pair<int, user_data_header>> user_data_info; //<番号,ユーザーデータ>
		};

		save_data_info save_data_info;

		bool load_save_data_information() const {
			//セーブデータ情報の読み込み
			if (const auto file = file_accessor->load_file(save_data_information_file_name); file == nullptr) {
				PE_LOG_ERROR("セーブデータ情報ファイルの取得に失敗しました。");
				return false;
			}
			//実装中

			return true;
		}

		bool load_common_save_data() {
			const auto file = file_accessor->load_file(common_save_data_file_name);
			if (file == nullptr) {
				PE_LOG_ERROR("共通セーブデータファイルの取得に失敗しました。");
				return false;
			}
			try {
				common_data = std::make_unique<util::data_container>(
					util::deserialize_data_container(file));
			} catch (std::runtime_error &) {
				PE_LOG_ERROR("共通セーブデータの読み込みに失敗しました。");
				return false;
			}
			return true;
		}

		bool load_user_save_data(int idx) {
			if (idx < 0 || save_data_info.user_save_data_count <= idx) {
				PE_LOG_ERROR("範囲外のユーザーセーブデータ番号が指定されました。(", idx, ")");
				return false;
			}
			const std::string file_name = std::string(user_save_data_file_name) + std::to_string(
				save_data_info.user_data_info[idx].first);
			const auto file = file_accessor->load_file(file_name);
			if (file == nullptr) {
				PE_LOG_ERROR("ユーザーセーブデータ", idx, "ファイルの取得に失敗しました。");
				return false;
			}
			try {
				current_user_data = std::make_unique<util::data_container>(
					util::deserialize_data_container(file));
			} catch (std::runtime_error &) {
				PE_LOG_ERROR("ユーザーセーブデータ", idx, "の読み込みに失敗しました。");
				return false;
			}
			return true;
		}

		bool save_save_data_information() {
			if (current_user_data == nullptr || save_data_info.user_save_data_count <= current_user_data_idx) {
				PE_LOG_ERROR("範囲外のユーザーセーブデータ番号が指定されました。(", current_user_data_idx, ")");
				return false;
			}
			assert(current_user_data != nullptr);
			const auto file = serialize_data_container(*current_user_data);
			if (file == nullptr) {
				PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx, "のファイル作成に失敗しました。");
				return false;
			}
			if (!file_accessor->save_file(
				std::string(user_save_data_file_name) + std::to_string(current_user_data_idx),
				*file)) {
				PE_LOG_ERROR("ユーザーセーブデータ", current_user_data_idx, "の保存に失敗しました。");
				return false;
			}
			return true;
		}

		// NOLINTNEXTLINE(modernize-use-nodiscard)
		bool save_common_save_data() const {
			const auto file = serialize_data_container(*common_data);
			if (file == nullptr) {
				PE_LOG_ERROR("共通セーブデータのファイル作成に失敗しました。");
				return false;
			}
			if (!file_accessor->save_file(common_save_data_file_name, *file)) {
				PE_LOG_ERROR("共通セーブデータの保存に失敗しました。");
				return false;
			}
			return true;
		}

		//bool save_current_user_save_data() { }
	};


	//////////////////////////////////////////////////////////////////////////
	//SaveDataManager
	//////////////////////////////////////////////////////////////////////////
	standard_save_manager::standard_save_manager() : impl_(std::make_unique<impl>()) {}

	standard_save_manager::~standard_save_manager() = default;

	bool standard_save_manager::initialize() {
		assert(impl_->file_accessor != nullptr);
		//セーブデータ情報の読み込み
		//if (!impl_->LoadSaveDataInformation()) { return false; }
		//共通セーブデータのロード
		//if (!impl_->LoadCommonSaveData()) { return false; }

		//PE_LOG_MESSAGE("セーブデータを正常に読み込みました。");
		PE_LOG_WARNING("SaveDataManagerは未実装です。");
		return true;
	}

	void standard_save_manager::finalize() {
		//Save();
	}

	void standard_save_manager::set_file_manipulator(const std::shared_ptr<file_manipulator> &file_accessor) {
		impl_->file_accessor = file_accessor;
	}

	void standard_save_manager::save() const {
		//共通データ
		impl_->save_common_save_data();
		//ユーザーデータ

		//セーブデータリスト更新
		impl_->save_save_data_information();
	}

	bool standard_save_manager::load_user_data(const int idx) const { return impl_->load_user_save_data(idx); }

	int standard_save_manager::get_user_data_count() const { return impl_->save_data_info.user_save_data_count; }

	int standard_save_manager::get_user_data_header_param_count() const {
		return impl_->save_data_info.user_save_data_param_count;
	}

	const util::data_container &standard_save_manager::get_common_data() const {
		assert(impl_->common_data != nullptr);
		return *impl_->common_data;
	}

	util::data_container &standard_save_manager::get_common_data() {
		assert(impl_->common_data != nullptr);
		return *impl_->common_data;
	}

	boost::optional<const util::data_container &> standard_save_manager::get_current_data() const {
		if (impl_->current_user_data_idx >= 0) {
			assert(impl_->current_user_data != nullptr);
			return *impl_->common_data;
		}
		return {};
	}

	boost::optional<util::data_container &> standard_save_manager::get_current_data() {
		if (impl_->current_user_data_idx >= 0) {
			assert(impl_->current_user_data != nullptr);
			return *impl_->common_data;
		}
		return {};
	}
}
