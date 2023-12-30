#pragma once

#include "..\core\resource_base.hpp"
#include "planeta/core/Vector2D.hpp"

namespace plnt {
	/*画像リソース*/
	class RGraph : public resource_base {
	public:
		RGraph() : _handle(-1) { };

		~RGraph() { };
		/*画像ハンドルを取得する。通常使うことはない。*/
		int GetHandle() const { return _handle; }
		/*画像サイズを取得*/
		const vector_2di &size() const { return image_size_; }
		/*内部サイズを取得。通常使うことはない。*/
		const vector_2di &internal_size() const { return internal_size_; }
		/*画像サイズの、内部サイズに対する割合を取得する。通常使うことはない。*/
		const vector_2dd &image_area() const { return image_area_; }

	private:
		int _handle;
		/*画像サイズ*/
		vector_2di image_size_;
		/*内部サイズ*/
		vector_2di internal_size_;
		/*画像サイズ/内部サイズ*/
		vector_2dd image_area_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		bool _AdjustImageSize(); //DXライブラリのDrawPolygon関数に合わせて、画像を8以上の2のべき乗数に合わせる
	};
}
