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
			const Vector2D<int>& size()const { return _image_size; }
			/*内部サイズを取得。通常使うことはない。*/
			const Vector2D<int>& internal_size()const { return _internal_size; }
		private:
			int _handle;
			/*画像サイズ*/
			Vector2D<int> _image_size;
			/*内部サイズ*/
			Vector2D<int> _internal_size;
			bool _Create(const std::shared_ptr<file_system::File>& file) override;
			void _Dispose() override;
			bool _AdjustImageSize(); //DXライブラリのDrawPolygon関数に合わせて、画像を8以上の2のべき乗数に合わせる
		};
	}
}
