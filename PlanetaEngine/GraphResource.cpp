#include "GraphResource.h"
#include "DxLib.h"
#include "File.h"
#include "MathUtility.h"
#include "SystemLog.h"
//#include "boost/lexical_cast.hpp"

namespace planeta_engine {
	namespace resources {
		bool GraphResource::_Create(const std::shared_ptr<const file_system::File>& file)
		{
			if (file->GetStatus() != file_system::File::FileStatus::Available) { return false; }
			_handle = CreateGraphFromMem(file->GetTopPointer(), file->GetSize(), nullptr, 0, 1, 0);
			if (_handle >= 0) {
				GetGraphSize(_handle, &_image_size.x, &_image_size.y);
				if (_AdjustImageSize() == false) {
					debug::SystemLog::instance().LogError("画像サイズの調整に失敗しました。", "GraphResource::_Create");
					return false;
				}
				return true;
			}
			else { 
				_image_size.Set(0, 0);
				_internal_size.Set(0, 0);
				debug::SystemLog::instance().LogError("画像リソースの読み込みに失敗しました。", "GraphResource::_Create");
				return false;
			}
		}

		void GraphResource::_Dispose()
		{
			if (_handle >= 0) {
				DeleteGraph(_handle);
				_handle = -1;
			}
		}

		bool GraphResource::_AdjustImageSize()
		{
			bool adjust_flag = false; //サイズ調整を行うか否かのフラグ
			_internal_size = _image_size;
			if (_image_size.x < 8) {
				_internal_size.x = 8;
				adjust_flag = true;
			}
			else if (math::is2Pow(_image_size.x) == false) {
				_internal_size.x = math::getMin2Pow(_image_size.x);
				adjust_flag = true;
			}
			if (_image_size.y < 8) {
				_internal_size.y = 8;
				adjust_flag = true;
			}
			else if (math::is2Pow(_image_size.y) == false) {
				_internal_size.y = math::getMin2Pow(_image_size.y);
				adjust_flag = true;
			}
			if (_internal_size.x >= 2048 || _internal_size.y >= 2048) {
				debug::SystemLog::instance().LogWarning(std::string("テクスチャサイズが2048以上です。デバイスによっては表示できない可能性があります。") + _internal_size.ToString(), "GraphResource::_AdjustImageSize");
			}
			//サイズが2の累乗でなかったら画像作成
			if (adjust_flag) {
				int buf_gh = MakeScreen(_internal_size.x, _internal_size.y, true);
				if (buf_gh < 0) { 
					debug::SystemLog::instance().LogError("バッファスクリーンの作成に失敗しました。", "GraphResourceComponent::_AdjustImageSize");
					return false; 
				}
				int cur_draw_scr = GetDrawScreen();
				bool error_flag = false;
				int texture_ghandle = MakeGraph(_internal_size.x, _internal_size.y);
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
				if (GetDrawScreenGraph(0, 0, _internal_size.x, _internal_size.y, texture_ghandle) != 0) {
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
}

