#include "GraphResource.h"
#include "DxLib.h"
#include "File.h"
#include "MathUtility.h"
#include "SystemLog.h"
//#include "boost/lexical_cast.hpp"

namespace planeta {
	bool GraphResource::_Create(const std::shared_ptr<const File>& file) {
		if (file->GetStatus() != File::FileStatus::Available) { return false; }
		_handle = CreateGraphFromMem(file->GetTopPointer(), file->GetSize(), nullptr, 0, 1, 0);
		if (_handle >= 0) {
			GetGraphSize(_handle, &image_size_.x, &image_size_.y);
			if (_AdjustImageSize() == false) {
				debug::SystemLog::instance().LogError("画像サイズの調整に失敗しました。", "GraphResource::_Create");
				return false;
			}
			image_area_.Set((double)image_size_.x / internal_size_.x, (double)image_size_.y / internal_size_.y);
			return true;
		} else {
			image_size_.Set(0, 0);
			internal_size_.Set(0, 0);
			image_area_.Set(0, 0);
			debug::SystemLog::instance().LogError("画像リソースの読み込みに失敗しました。", "GraphResource::_Create");
			return false;
		}
	}

	void GraphResource::_Dispose() {
		if (_handle >= 0) {
			DeleteGraph(_handle);
			_handle = -1;
		}
	}

	bool GraphResource::_AdjustImageSize() {
		bool adjust_flag = false; //サイズ調整を行うか否かのフラグ
		internal_size_ = image_size_;
		if (image_size_.x < 8) {
			internal_size_.x = 8;
			adjust_flag = true;
		} else if (math::is2Pow(image_size_.x) == false) {
			internal_size_.x = math::getMin2Pow(image_size_.x);
			adjust_flag = true;
		}
		if (image_size_.y < 8) {
			internal_size_.y = 8;
			adjust_flag = true;
		} else if (math::is2Pow(image_size_.y) == false) {
			internal_size_.y = math::getMin2Pow(image_size_.y);
			adjust_flag = true;
		}
		if (internal_size_.x >= 2048 || internal_size_.y >= 2048) {
			debug::SystemLog::instance().LogWarning(std::string("テクスチャサイズが2048以上です。デバイスによっては表示できない可能性があります。") + internal_size_.ToString(), "GraphResource::_AdjustImageSize");
		}
		//サイズが2の累乗でなかったら画像作成
		if (adjust_flag) {
			int buf_gh = MakeScreen(internal_size_.x, internal_size_.y, true);
			if (buf_gh < 0) {
				debug::SystemLog::instance().LogError("バッファスクリーンの作成に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
				return false;
			}
			int cur_draw_scr = GetDrawScreen();
			bool error_flag = false;
			int texture_ghandle = MakeGraph(internal_size_.x, internal_size_.y);
			if (texture_ghandle < 0) {
				debug::SystemLog::instance().LogError("新しい画像データの作成に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
				error_flag = true;
			}
			if (SetDrawScreen(buf_gh) != 0) {
				debug::SystemLog::instance().LogError("描画先の切り替えに失敗しました。", "GraphResourceComponent::_AdjustImageSize");
				error_flag = true;
			}
			if (DrawGraph(0, 0, _handle, true) != 0) {
				debug::SystemLog::instance().LogError("画像データの描画に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
				error_flag = true;
			}
			if (GetDrawScreenGraph(0, 0, internal_size_.x, internal_size_.y, texture_ghandle) != 0) {
				debug::SystemLog::instance().LogError("描画データの取得に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
				error_flag = true;
			}
			if (SetDrawScreen(cur_draw_scr) != 0) {
				debug::SystemLog::instance().LogError("描画先の復元に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
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

