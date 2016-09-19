#pragma once
#include"Vector2D.h"

namespace planeta {

	template <typename T>
	class RectAngle {
	public:
		RectAngle();
		RectAngle(const Vector2D<T>&, const Vector2D<T>&);
		RectAngle(T, T, T, T);
		~RectAngle() = default;
		T left()const;
		T right()const;
		T top()const;
		T bottom()const;
		T height()const;
		T width()const;
		void Set(const Vector2D<T>&, const Vector2D<T>&);
		void Set(T, T, T, T);
		Vector2D<T> position;
		Vector2D<T> size;
	};

	template<typename T>
	RectAngle<T>::RectAngle() :position(), size(1, 1) {}

	template<typename T>
	RectAngle<T>::RectAngle(const Vector2D<T>& p, const Vector2D<T>& s) : position(p), size(s) {}

	template<typename T>
	RectAngle<T>::RectAngle(T x, T y, T w, T h) : position(x, y), size(w, h) {}

	template<typename T>
	void RectAngle<T>::Set(const Vector2D<T>& p, const Vector2D<T>& s) { position = p, size = s; }

	template<typename T>
	void RectAngle<T>::Set(T x, T y, T w, T h) { position.Set(x, y), size.Set(w, h); }

	template<typename T>
	T RectAngle<T>::left()const { return position.x; }

	template<typename T>
	T RectAngle<T>::right()const { return position.x + size.x - 1; }

	template<typename T>
	T RectAngle<T>::top()const { return position.y; }

	template<typename T>
	T RectAngle<T>::bottom()const { return position.y + size.y - 1; }

	template<typename T>
	T RectAngle<T>::width()const { return size.x; }

	template<typename T>
	T RectAngle<T>::height()const { return size.y; }
}

#ifdef PE_ENABLE_REFLECTION_SYSTEM

namespace planeta {
	namespace util {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		template<typename T>
		void ReflectivePtreeConverter(RectAngle<T>& dst, const boost::property_tree::ptree& src) {
			if (src.size() != 2) {
				throw reflection_error(ConvertAndConnectToString("要素数が", src.size(), "ですが、RectAngleでは2である必要があります。"));
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