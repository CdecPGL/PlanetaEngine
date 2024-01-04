#pragma once

#include "../core/resource_base.hpp"
#include "../core/vector_2d.hpp"

namespace plnt {
	/*画像リソース*/
	class r_graph final : public resource_base {
	public:
		r_graph() : handle_(-1) {}
		~r_graph() override = default;
		/*画像ハンドルを取得する。通常使うことはない。*/
		[[nodiscard]] int get_handle() const { return handle_; }
		/*画像サイズを取得*/
		[[nodiscard]] const vector_2di &size() const { return image_size_; }
		/*内部サイズを取得。通常使うことはない。*/
		[[nodiscard]] const vector_2di &internal_size() const { return internal_size_; }
		/*画像サイズの、内部サイズに対する割合を取得する。通常使うことはない。*/
		[[nodiscard]] const vector_2dd &image_area() const { return image_area_; }

	private:
		int handle_;
		/*画像サイズ*/
		vector_2di image_size_;
		/*内部サイズ*/
		vector_2di internal_size_;
		/*画像サイズ/内部サイズ*/
		vector_2dd image_area_;
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		bool adjust_image_size(); //DXライブラリのDrawPolygon関数に合わせて、画像を8以上の2のべき乗数に合わせる
	};
}
