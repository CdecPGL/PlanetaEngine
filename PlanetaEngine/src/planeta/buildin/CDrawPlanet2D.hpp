#pragma once

#include <array>
#include <vector>

#include "planeta/core/WeakPointer.hpp"
#include "planeta/core/Vertex2D.hpp"

#include "CDraw2D.hpp"

namespace plnt {
	namespace private_ {
		class graph_draw_data_2d;
	}

	/*! 惑星描画コンポーネント。
	
		@note DrawPlanetComponentのseparationはPlanetComponentのseparationとは異なる
	*/
	class CPlanet;

	class CDrawPlanet2D final : public CDraw2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawPlanet2D);

	public:
		using super = CDraw2D;
		CDrawPlanet2D();
		~CDrawPlanet2D();
		/*! 画像リソースを設定する*/
		bool SetGraphResource(const std::string &resource_id);

		/*! 画像のマッピングモードを表す列挙体*/
		enum class TextureMappingMode { Round, Plain };

		/*アクセサ*/
		/*! 画像リソースをIDで設定する*/
		CDrawPlanet2D &graph_resource_id(const std::string &res_id);
		/*! 水平方向の分割数を取得する*/
		unsigned int horizontal_separation() const { return _horizontal_separation; }
		/*! 水平方向の分割数を設定する*/
		CDrawPlanet2D &horizontal_separation(unsigned int sep);
		/*! 垂直方向の分割数を取得する*/
		unsigned int vertical_separation() const { return _vertical_separation; }
		/*! 垂直方向の分割数を設定する*/
		CDrawPlanet2D &vertical_separation(unsigned int sep);
		/*! 画像のマッピングモードを取得する*/
		TextureMappingMode texture_mapping_mode() const { return tex_map_mode_; }
		/*! 画像のマッピングモードを設定する*/
		void texture_mapping_mode(TextureMappingMode tmm);

	private:
		/*水平分割数*/
		unsigned int _horizontal_separation;
		/*垂直分割数*/
		unsigned int _vertical_separation;
		/*ポリゴンの設定が必要か*/
		bool set_polygon_flag_ = false;
		/*画像描画データ*/
		std::shared_ptr<private_::graph_draw_data_2d> graph_draw_data_;
		/*テクスチャマッピングモード*/
		TextureMappingMode tex_map_mode_;
		virtual bool get_other_components_proc(const go_component_getter &com_getter) override;
		virtual void on_initialized() override;
		virtual void on_finalized() noexcept override;
		non_owing_pointer<CPlanet> _planet_component;
		/*ポリゴンセット*/
		void (CDrawPlanet2D::*polygon_setter_)();
		void SetPolygon_();
		void SetPolygonRoundly_();
		void SetPolygonPlainly_();
		/*ポリゴン情報更新*/
		void (CDrawPlanet2D::*polygon_updater_)();
		void UpdatePolygon_();
		void UpdatePolygonRoundly_();
		void UpdatePolygonPlainly_();
		/*描画処理*/
		void DrawProc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawPlanet2D);

	namespace reflection {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		template <>
		struct reflective_ptree_converter_impl<CDrawPlanet2D::TextureMappingMode> {
			void operator()(CDrawPlanet2D::TextureMappingMode &dst, const boost::property_tree::ptree &src) const {
				try {
					if (const auto str = src.get_value<std::string>(); str == "Round") {
						dst = CDrawPlanet2D::TextureMappingMode::Round;
					} else if (str == "Plain") { dst = CDrawPlanet2D::TextureMappingMode::Plain; } else {
						throw reflection_error(util::ConvertAndConnectToString(
							"\"", src.get_value<std::string>(), "\"は\"",
							typeid(CDrawPlanet2D::TextureMappingMode).name(), "\"のメンバーではありません。"));
					}
				} catch (boost::property_tree::ptree_bad_data &e) { throw reflection_error(e.what()); }
			}
		};
	}
}
