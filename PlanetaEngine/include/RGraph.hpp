#pragma once
#include "ResourceBase.hpp"
#include "Vector2D.hpp"

namespace planeta {
	/*画像リソース*/
	class RGraph : public ResourceBase {
	public:
		RGraph() :_handle(-1) {};
		~RGraph() {};
		/*画像ハンドルを取得する。通常使うことはない。*/
		int GetHandle()const { return _handle; }
		/*画像サイズを取得*/
		const Vector2Di& size()const { return image_size_; }
		/*内部サイズを取得。通常使うことはない。*/
		const Vector2Di& internal_size()const { return internal_size_; }
		/*画像サイズの、内部サイズに対する割合を取得する。通常使うことはない。*/
		const Vector2Dd& image_area()const { return image_area_; }
	private:
		int _handle;
		/*画像サイズ*/
		Vector2Di image_size_;
		/*内部サイズ*/
		Vector2Di internal_size_;
		/*画像サイズ/内部サイズ*/
		Vector2Dd image_area_;
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		bool _AdjustImageSize(); //DXライブラリのDrawPolygon関数に合わせて、画像を8以上の2のべき乗数に合わせる
	};
}
