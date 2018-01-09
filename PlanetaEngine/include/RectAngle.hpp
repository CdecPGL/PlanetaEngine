#pragma once
#include"Vector2D.hpp"

//windows.hのmin、maxマクロを無効化
#undef min
#undef max

namespace planeta {
	/*! 矩形を表すクラス*/
	template <typename T>
	class RectAngle final{
	public:
		/*! コンストラクタ*/
		constexpr RectAngle() = default;
		/*! XY位置とサイズを指定するコンストラクタ*/
		constexpr RectAngle(const Vector2D<T>& position, const Vector2D<T>& size) :position_(position), size_(size) {}
		/*! XY位置とサイズを指定するコンストラクタ*/
		constexpr RectAngle(T x, T y, T width, T height) : position_(x, y), size_(width, height) {}
		/*! デストラクタ*/
		~RectAngle() = default;
		/*! XY位置とサイズを設定する*/
		void Set(const Vector2D<T>& position, const Vector2D<T>& size) {
			position_ = position;
			size_ = size;
		}
		/*! XY位置とサイズを設定する*/
		void Set(T x, T y, T width, T height) {
			position_.Set(x, y);
			size_.Set(width, height);
		}
		/*! 最小のX座標を取得*/
		constexpr T x_min()const { return position_.x; }
		/*! 最小のX座標を設定。最大のX座標は維持される*/
		void x_min(T p_x_min) { 
			size_.x += position_.x - p_x_min;
			position_.x = p_x_min; 
		}
		/*! 最大のX座標を取得*/
		constexpr T x_max()const { return position_.x + size_.x - 1; }
		/*! 最大のX座標を設定。最小のX座標は維持される*/
		void x_max(T p_x_max) { size_.x = p_x_max - position_.x + 1; }
		/*! 最小のY座標を取得*/
		constexpr T y_min()const { return position_.y; }
		/*! 最小のY座標を設定。最小のY座標は維持される*/
		void y_min(T p_y_min) { 
			size_.y += position_.y - p_y_min; 
			position_.y = p_y_min;
		}
		/*! 最大のY座標を取得*/
		constexpr T y_max()const { return position_.y + size_.y - 1; }
		/*! 最大のY座標を設定。最小のY座標は維持される*/
		void y_max(T p_y_max) { size_.y = p_y_max - position_.y + 1; }
		/*! 最小隅を取得*/
		constexpr Vector2D<T> min()const { return position_; }
		/*! 最小隅を設定。最大隅は維持される*/
		void min(const Vector2D<T>& p_min) { 
			size_ += position_ - p_min;
			position_ = p_min; 
		}
		/*! 最大隅を取得*/
		constexpr Vector2D<T> max()const { return position_ + size_ - Vector2D<T>{1, 1}; }
		/*! 最大隅を設定。最小隅は維持される*/
		void max(const Vector2D<T>& p_max) { size_ = p_max - position_ + Vector2D<T>{1, 1}; }
		/*! 幅を取得*/
		constexpr T height()const { return size_.x; }
		/*! 幅を設定。X位置を基準とする*/
		void height(T p_height) { size_.x = p_height; }
		/*! 高さを取得*/
		constexpr T width()const { return size_.y; }
		/*! 高さを設定。Y位置を基準とする*/
		void width(T p_width) { size_.y = p_width; }
		/*! サイズを取得*/
		constexpr Vector2D<T> size()const { return size_; }
		/*! サイズを設定。XY位置を基準とする*/
		void size(const Vector2D<T> p_size) { size_ = size; }
		/*! 中心を取得*/
		constexpr Vector2D<T> center()const { return position_ + size_ / 2; }
		/*! 中心を設定*/
		void center(const Vector2D<T> p_center) {
			position_ = p_center - size_ / 2;
		}
		/*! X位置を取得。x_minと同じ*/
		constexpr T x()const { return position_.x; }
		/*! X位置を設定。基本的にx_minと同じだが、サイズが維持される*/
		void x(T p_x) { position_.x = p_x; }
		/*! Y位置を取得。y_minと同じ*/
		constexpr T y()const { return position_.y; }
		/*! Y位置を設定。基本的にy_minと同じだが、サイズが維持される*/
		void y(T p_y) { position_.y = p_y; }
		/*! XY位置を取得。minと同じ*/
		constexpr Vector2D<T> position()const { return position_; }
		/*! XY位置を設定。基本的にminと同じだが、サイズが維持される*/
		void position(const Vector2D<T>& p_position) { position_ = p_position; }
	private:
		Vector2D<T> position_;
		Vector2D<T> size_;
	};
	using RectAnglei = RectAngle<int32_t>; //32bit符号あり整数型RectAngle
	using RectAnglef = RectAngle<float>; //単精度浮動少数型RectAngle
	using RectAngled = RectAngle<double>; //倍精度浮動少数型RectAngle
}

#ifdef PE_ENABLE_REFLECTION_SYSTEM

namespace planeta {
	namespace util {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		template<typename T>
		void ReflectivePtreeConverter(RectAngle<T>& dst, const boost::property_tree::ptree& src) {
			if (src.size() != 2) {
				throw reflection_error(ConvertAndConnectToString("要素数が", src.size(), "ですが、RectAngleでは2である必要があります。[[position_x,position_y],[width,height]]のように指定してください。"));
			}
			size_t idx = 0;
			std::array<Vector2D<T>, 2> ary; //座標とサイズ
			for (auto&& pp : src) {
				if (pp.first.empty() == false) {
					throw planeta::reflection_error(planeta::util::ConvertAndConnectToString("Vector2DのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")")); \
				}
				ReflectivePtreeConverter(ary[idx++], pp.second);
			}
			dst.Set(ary[0], ary[1]);
		}
	}
}

#endif