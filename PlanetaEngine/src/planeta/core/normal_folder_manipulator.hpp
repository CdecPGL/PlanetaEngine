#pragma once

#include "file_manipulator.hpp"

namespace plnt {
	class normal_folder_manipulator final : public file_manipulator {
	public:
		using file_manipulator::file_manipulator;
		normal_folder_manipulator(const normal_folder_manipulator &) = delete;
		normal_folder_manipulator(normal_folder_manipulator &&) = delete;
		~normal_folder_manipulator() override = default;
		normal_folder_manipulator &operator=(const normal_folder_manipulator &) = delete;
		normal_folder_manipulator &operator=(normal_folder_manipulator &&) = delete;

	private:
		bool load_file_by_path(file &, const std::string &) const;
		static int load_data_core(file &, const std::string &);
		bool open_proc(const std::string &path) override;
		void close_proc() override;
		bool load_file_proc(const std::string &, file &file) override;
		bool save_file_proc(const std::string &name, const file &file) override;
		[[nodiscard]] size_t get_file_count_proc() const override;
		[[nodiscard]] bool check_file_existence_proc(const std::string &p_path) const override;
		bool get_all_file_paths_proc(std::vector<std::string> &path_list) const override;
	};
}
