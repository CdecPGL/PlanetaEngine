#pragma once

#include <string>
#include <array>
#include "Vector2D.h"
#include "CollisionGroupMatrix.h"

namespace planeta {
	class IConfigManager {
	public:
		virtual ~IConfigManager() = 0 {};
		/*! ゲームタイトルを取得する*/
		virtual std::string game_title()const = 0;
		/*! ゲームのバージョンを表す文字列を取得する*/
		virtual std::string game_version_string()const = 0;
		/*! ゲームのバージョンを表す数値配列を取得する[Major, Minor, Sub]*/
		virtual std::array<int, 3> game_version_numbers()const = 0;
		/*! ウインドウモードかどうかのフラグを取得する*/
		virtual bool is_window_mode()const = 0;
		/*! 描画サイズを取得する。プログラム内での画面の広さを表す*/
		virtual Vector2Di draw_size()const = 0;
		/*! ウインドウサイズを取得する。表示される画面のサイズを表す*/
		virtual Vector2Di window_size()const = 0;
		/*! 衝突グループマトリクスを取得する*/
		virtual const CollisionGroupMatrix& collision_group_matrix()const = 0;
		/*! Cursorを表示するかどうかを取得する*/
		virtual bool is_cursor_visible()const = 0;
	};
}
