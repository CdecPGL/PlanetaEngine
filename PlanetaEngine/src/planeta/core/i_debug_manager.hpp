#pragma once

#include <functional>

#include "StringUtility.hpp"
#include "Vector2D.hpp"
#include "color.hpp"

namespace plnt {
	/*! デバッグ情報追加クラス公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_debug_information_adder {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_debug_information_adder() = 0 { }
		/*! 文字列で行を追加する*/
		virtual void add_line(std::string line) = 0;
		/*! 任意の引数で行を追加する*/
		template <typename... Args>
		void add_line_v(Args &&... args) { add_line(util::ConvertAndConnectToString(args...)); }
	};

	/*! デバッグ描画クラス公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_debug_drawer {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_debug_drawer() = 0 { }
		/*! 線を描画する*/
		virtual void draw_line(const vector_2df &s_pos, const vector_2df &e_pos, const color &color) = 0;
		/*! 円を描画する*/
		virtual void draw_circle(const vector_2df &pos, float radius, const color &color, bool filled) = 0;
		/*! 複数の線を描画する*/
		virtual void draw_lines(const std::vector<vector_2df> &pos_list, const color &color) = 0;
	};

	/*! デバッグマネージャ公開インターフェイス*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_debug_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_debug_manager() = 0 { }
		/*! デバッグ情報チャンネルを作成する*/
		virtual bool create_debug_information_channel(const std::string &channel_id,
		                                              const std::function<void(i_debug_information_adder &)> &handler) =
		0;
		/*! デバッグ情報チャンネルを削除する*/
		virtual bool delete_debug_information_channel(const std::string &channel_id) = 0;
		/*! デバッグ描画用チャンネルを作成する*/
		virtual bool create_debug_draw_channel(const std::string &channel_id,
		                                       std::function<void(i_debug_drawer &)> handler) = 0;
		/*! デバッグ描画用チャンネルを削除する*/
		virtual bool delete_debug_draw_channel(const std::string &channel_id) = 0;
	};
}
