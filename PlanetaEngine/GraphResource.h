#pragma once
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace resources {
		/*画像リソース*/
		class GraphResource : public core::ResourceBase {
		public:
			GraphResource() :_handle(-1){};
			~GraphResource() {};
			/*画像ハンドルを取得する。通常使うことはない。*/
			int GetHandle()const { return _handle; }
			/*画像サイズを取得*/
			const math::Vector2Di& size()const { return image_size_; }
			/*内部サイズを取得。通常使うことはない。*/
			const math::Vector2Di& internal_size()const { return internal_size_; }
			/*画像サイズの、内部サイズに対する割合を取得する。通常使うことはない。*/
			const math::Vector2Dd& image_area()const { return image_area_; }
		private:
			int _handle;
			/*画像サイズ*/
			math::Vector2Di image_size_;
			/*内部サイズ*/
			math::Vector2Di internal_size_;
			/*画像サイズ/内部サイズ*/
			math::Vector2Dd image_area_;
			bool _Create(const std::shared_ptr<const file_system::File>& file) override;
			void _Dispose() override;
			bool _AdjustImageSize(); //DXライブラリのDrawPolygon関数に合わせて、画像を8以上の2のべき乗数に合わせる
		};
	}
}
