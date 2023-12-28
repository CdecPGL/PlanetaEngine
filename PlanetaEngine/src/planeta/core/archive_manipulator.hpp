#pragma once
#include "file_manipulator.hpp"

class Extracter;

namespace plnt {
	class archive_manipulator final : public file_manipulator {
	public:
		archive_manipulator();
		archive_manipulator(const archive_manipulator &) = delete;
		archive_manipulator(archive_manipulator &&) = delete;
		~archive_manipulator() override;
		archive_manipulator &operator=(const archive_manipulator &) = delete;
		archive_manipulator &operator=(archive_manipulator &&) = delete;

		void set_key(unsigned int); //復号化キーセット
	private:
		bool open_proc(const std::string &path) override;
		void close_proc() override;
		//std::unique_ptr<Extracter> _extracter;
		unsigned int key_; //復号化キー
		bool reload_proc() override;
		bool load_file_proc(const std::string &, File &file) override;
		bool save_file_proc(const std::string &name, const File &file) override { return false; }
		[[nodiscard]] bool check_file_existence_proc(const std::string &path) const override;
		[[nodiscard]] size_t get_file_count_proc() const override;
		bool get_all_file_paths_proc(std::vector<std::string> &path_list) const override;
	};
}
