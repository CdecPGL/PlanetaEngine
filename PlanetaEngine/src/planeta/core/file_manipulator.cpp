#include "file_manipulator.hpp"
#include "LogUtility.hpp"
#include "../archiver/encrypter_base.hpp"

namespace plnt {
	namespace {
		namespace access_mode {
			using type = uint_fast32_t;
			constexpr type none = 0b00;
			constexpr type read = 0b01;
			constexpr type write = 0b10;
			constexpr type read_write = 0b11;
		}

		constexpr access_mode::type convert_access_mode_to_uint32(const AccessMode mode) {
			switch (mode) {
				case AccessMode::ReadOnly:
					return access_mode::read;
				case AccessMode::WriteOnly:
					return access_mode::write;
				case AccessMode::ReadWrite:
					return access_mode::read_write;
				case AccessMode::Invalid:
					return access_mode::none;
			}
			return access_mode::none;
		}
	}

	bool file_manipulator::check_file_exist(const std::string &path) const { return check_file_existence_proc(path); }

	std::vector<std::string> file_manipulator::get_all_file_paths() const {
		if (std::vector<std::string> out{}; get_all_file_paths_proc(out)) { return out; }
		PE_LOG_ERROR("ファイルパスリストの取得に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
		return {};
	}

	AccessMode file_manipulator::mode() const { return mode_; }

	boost::optional<const archiver::encrypter_base &> file_manipulator::encrypter() const & {
		if (encrypter_) { return *encrypter_; }
		return boost::none;
	}

	std::shared_ptr<File> file_manipulator::load_file(const std::string &name) {
		if (is_opened() == false) {
			PE_LOG_ERROR("無効なファイルマニピュレータです。初期化されていないか、破棄された可能性があります。");
			return nullptr;
		}
		if ((convert_access_mode_to_uint32(mode_) & access_mode::read) != access_mode::read) {
			PE_LOG_ERROR("読み込み属性がありません。value:", convert_access_mode_to_uint32(mode_));
			return nullptr;
		}
		if (auto file = std::make_shared<File>(); load_file_proc(name, *file)) {
			file->SetFileName(name);
			return file;
		}
		PE_LOG_ERROR("ファイル", name, "の読み込みに失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
		return nullptr;
	}

	file_manipulator::file_manipulator() : is_opened_(false), auto_create_(false) { }
	file_manipulator::~file_manipulator() = default;

	bool file_manipulator::open(const std::string &path, const AccessMode access_mode, const bool auto_create) {
		if (is_opened_) {
			PE_LOG_ERROR("すでに開かれています。");
			return false;
		}
		mode_ = access_mode;
		auto_create_ = auto_create;
		root_path_ = path;
		if (open_proc(path)) {
			is_opened_ = true;
			PE_LOG_MESSAGE("初期化されました。(パス ", root_path(), ",ファイル数 ", get_file_count_proc(), ",タイプ ",
			               typeid(*this).name(),
			               ")");
			return true;
		} else {
			PE_LOG_ERROR("初期化に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			mode_ = AccessMode::Invalid;
			auto_create_ = false;
			root_path_ = "";
			encrypter_ = nullptr;
			return false;
		}
	}

	bool file_manipulator::open(const std::string &path, const AccessMode access_mode,
	                            std::unique_ptr<const archiver::encrypter_base> &&encrypter, const bool auto_create) {
		encrypter_ = std::move(encrypter);
		return open(path, access_mode, auto_create);
	}

	void file_manipulator::close() {
		if (!is_opened_) {
			PE_LOG_WARNING("開かれる前に閉じられようとしました。");
			return;
		}
		close_proc();
		mode_ = AccessMode::Invalid;
		auto_create_ = false;
		root_path_ = "";
		encrypter_ = nullptr;
		is_opened_ = false;
	}

	bool file_manipulator::save_file(const std::string &name, const File &file) {
		if (is_opened() == false) {
			PE_LOG_ERROR("無効なファイルマニピュレータです。初期化されていないか、破棄された可能性があります。");
			return false;
		}
		if ((convert_access_mode_to_uint32(mode_) & access_mode::write) != access_mode::write) {
			PE_LOG_ERROR("書き込み属性がありません。value:", convert_access_mode_to_uint32(mode_));
			return false;
		}
		if (save_file_proc(name, file)) { return true; } else {
			PE_LOG_ERROR("ファイル", name, "の保存に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		}
	}

	bool file_manipulator::reload() {
		if (!reload_proc()) {
			PE_LOG_ERROR("管理情報の更新に失敗しました。(パス ", root_path(), ",タイプ ", typeid(*this).name(), ")");
			return false;
		} else { return true; }
	}
}
