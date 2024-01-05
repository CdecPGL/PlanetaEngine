#include "DxLib.h"

#include "planeta/core/file.hpp"
#include "../core/log_utility.hpp"
#include "../math/math_utility.hpp"
#include "r_graph.hpp"

namespace plnt {
	bool r_graph::on_loaded(const file &file, [[maybe_unused]] const json_file &metadata, [[maybe_unused]] resource_referencer &referencer) {
		handle_ = CreateGraphFromMem(file.top_pointer(), file.size(), nullptr, 0, 1, 0);
		if (handle_ >= 0) {
			GetGraphSize(handle_, &image_size_.x, &image_size_.y);
			if (adjust_image_size() == false) {
				PE_LOG_ERROR("画像サイズの調整に失敗しました。");
				return false;
			}
			image_area_.set(static_cast<double>(image_size_.x) / internal_size_.x, static_cast<double>(image_size_.y) / internal_size_.y);
			return true;
		}
		image_size_.set(0, 0);
		internal_size_.set(0, 0);
		image_area_.set(0, 0);
		PE_LOG_ERROR("画像リソースの読み込みに失敗しました。");
		return false;
	}

	void r_graph::on_disposed() {
		if (handle_ >= 0) {
			DeleteGraph(handle_);
			handle_ = -1;
		}
	}

	bool r_graph::adjust_image_size() {
		bool adjust_flag = false; //サイズ調整を行うか否かのフラグ
		internal_size_ = image_size_;
		if (image_size_.x < 8) {
			internal_size_.x = 8;
			adjust_flag = true;
		} else if (math::is2_pow(image_size_.x) == false) {
			internal_size_.x = math::get_min2_pow(image_size_.x);
			adjust_flag = true;
		}
		if (image_size_.y < 8) {
			internal_size_.y = 8;
			adjust_flag = true;
		} else if (math::is2_pow(image_size_.y) == false) {
			internal_size_.y = math::get_min2_pow(image_size_.y);
			adjust_flag = true;
		}
		if (internal_size_.x >= 2048 || internal_size_.y >= 2048) {
			PE_LOG_WARNING("テクスチャサイズが2048以上です。デバイスによっては表示できない可能性があります。size : ", internal_size_.to_string());
		}
		//サイズが2の累乗でなかったら画像作成
		if (adjust_flag) {
			const int buf_gh = MakeScreen(internal_size_.x, internal_size_.y, true);
			if (buf_gh < 0) {
				PE_LOG_ERROR("バッファスクリーンの作成に失敗しました。");
				return false;
			}
			const int cur_draw_scr = GetDrawScreen();
			bool error_flag = false;
			const int texture_handle = MakeGraph(internal_size_.x, internal_size_.y);
			if (texture_handle < 0) {
				PE_LOG_ERROR("新しい画像データの作成に失敗しました。");
				error_flag = true;
			}
			if (SetDrawScreen(buf_gh) != 0) {
				PE_LOG_ERROR("描画先の切り替えに失敗しました。");
				error_flag = true;
			}
			if (DrawGraph(0, 0, handle_, true) != 0) {
				PE_LOG_ERROR("画像データの描画に失敗しました。");
				error_flag = true;
			}
			if (GetDrawScreenGraph(0, 0, internal_size_.x, internal_size_.y, texture_handle) != 0) {
				PE_LOG_ERROR("描画データの取得に失敗しました。");
				error_flag = true;
			}
			if (SetDrawScreen(cur_draw_scr) != 0) {
				PE_LOG_ERROR("描画先の復元に失敗しました。");
				error_flag = true;
			}
			DeleteGraph(buf_gh);
			DeleteGraph(handle_);
			if (error_flag) { return false; }
			handle_ = texture_handle;
		}
		return true;
	}
}
