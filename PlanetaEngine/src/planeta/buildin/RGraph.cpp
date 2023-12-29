#include "DxLib.h"

#include "planeta/core/file.hpp"
#include "..\core\log_utility.hpp"
#include "..\math\math_utility.hpp"

#include "RGraph.hpp"

namespace plnt {
	bool RGraph::OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) {
		_handle = CreateGraphFromMem(file.top_pointer(), file.size(), nullptr, 0, 1, 0);
		if (_handle >= 0) {
			GetGraphSize(_handle, &image_size_.x, &image_size_.y);
			if (_AdjustImageSize() == false) {
				PE_LOG_ERROR("画像サイズの調整に失敗しました。");
				return false;
			}
			image_area_.Set((double)image_size_.x / internal_size_.x, (double)image_size_.y / internal_size_.y);
			return true;
		} else {
			image_size_.Set(0, 0);
			internal_size_.Set(0, 0);
			image_area_.Set(0, 0);
			PE_LOG_ERROR("画像リソースの読み込みに失敗しました。");
			return false;
		}
	}

	void RGraph::OnDisposed() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	bool RGraph::_AdjustImageSize() {
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
			PE_LOG_WARNING("テクスチャサイズが2048以上です。デバイスによっては表示できない可能性があります。size : ", internal_size_.ToString());
		}
		//サイズが2の累乗でなかったら画像作成
		if (adjust_flag) {
			int buf_gh = MakeScreen(internal_size_.x, internal_size_.y, true);
			if (buf_gh < 0) {
				PE_LOG_ERROR("バッファスクリーンの作成に失敗しました。");
				return false;
			}
			int cur_draw_scr = GetDrawScreen();
			bool error_flag = false;
			int texture_ghandle = MakeGraph(internal_size_.x, internal_size_.y);
			if (texture_ghandle < 0) {
				PE_LOG_ERROR("新しい画像データの作成に失敗しました。");
				error_flag = true;
			}
			if (SetDrawScreen(buf_gh) != 0) {
				PE_LOG_ERROR("描画先の切り替えに失敗しました。");
				error_flag = true;
			}
			if (DrawGraph(0, 0, _handle, true) != 0) {
				PE_LOG_ERROR("画像データの描画に失敗しました。");
				error_flag = true;
			}
			if (GetDrawScreenGraph(0, 0, internal_size_.x, internal_size_.y, texture_ghandle) != 0) {
				PE_LOG_ERROR("描画データの取得に失敗しました。");
				error_flag = true;
			}
			if (SetDrawScreen(cur_draw_scr) != 0) {
				PE_LOG_ERROR("描画先の復元に失敗しました。");
				error_flag = true;
			}
			DeleteGraph(buf_gh);
			DeleteGraph(_handle);
			if (error_flag) { return false; }
			_handle = texture_ghandle;
		}
		return true;
	}
}
