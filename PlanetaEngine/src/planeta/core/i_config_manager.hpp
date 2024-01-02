#pragma once

#include <string>
#include <array>

#include "vector_2d.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "collision_group_matrix.hpp"

namespace plnt {
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_config_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_config_manager() = 0 { }

		/*! ゲームタイトルを取得する*/
		[[nodiscard]] virtual std::string game_title() const = 0;
		/*! ゲームのバージョンを表す文字列を取得する*/
		[[nodiscard]] virtual std::string game_version_string() const = 0;
		/*! ゲームのバージョンを表す数値配列を取得する[Major, Minor, Sub]*/
		[[nodiscard]] virtual std::array<int, 3> game_version_numbers() const = 0;
		/*! ウインドウモードかどうかのフラグを取得する*/
		[[nodiscard]] virtual bool is_window_mode() const = 0;
		/*! 描画サイズを取得する。プログラム内での画面の広さを表す*/
		[[nodiscard]] virtual vector_2di draw_size() const = 0;
		/*! ウインドウサイズを取得する。表示される画面のサイズを表す*/
		[[nodiscard]] virtual vector_2di window_size() const = 0;
		/*! 衝突グループマトリクスを取得する*/
		[[nodiscard]] virtual const collision_group_matrix &collision_group_matrix() const = 0;
		/*! Cursorを表示するかどうかを取得する*/
		[[nodiscard]] virtual bool is_cursor_visible() const = 0;
	};
}
