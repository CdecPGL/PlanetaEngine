#include "Game.hpp"
#include "CDrawPlanet2D.hpp"
#include <cmath>
#include "ScreenDrawer2D.hpp"
#include "IResourceManager.hpp"
#include "RGraph.hpp"
#include "IGameObject.hpp"
#include "CPlanet.hpp"
#include "math/MathConstant.hpp"
#include "LogUtility.hpp"
#include "GraphDrawData2D.hpp"

namespace {
	constexpr unsigned int kDefaultHorizontalSeparation(90);
	constexpr unsigned int kDefaultVerticalSeparation(5);
}

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawPlanet2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawPlanet2D, horizontal_separation)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawPlanet2D, vertical_separation)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawPlanet2D, texture_mapping_mode)
			.WriteOnlyProperty("graph_resource_id", &CDrawPlanet2D::graph_resource_id)
			.ShallowCopyTarget(&CDrawPlanet2D::_horizontal_separation)
			.ShallowCopyTarget(&CDrawPlanet2D::_vertical_separation)
			.DeepCopyTarget(&CDrawPlanet2D::graph_draw_data_)
			.CopyHandler([](const CDrawPlanet2D& src, CDrawPlanet2D& dst) {
			dst.texture_mapping_mode(src.texture_mapping_mode());
		});
	}

	CDrawPlanet2D::CDrawPlanet2D() :_horizontal_separation(kDefaultHorizontalSeparation), _vertical_separation(kDefaultVerticalSeparation), graph_draw_data_(std::make_shared<private_::GraphDrawData2D>()) {
		texture_mapping_mode(TextureMappingMode::Round);
	}

	CDrawPlanet2D::~CDrawPlanet2D() = default;

	void CDrawPlanet2D::DrawProc(ScreenDrawer2D& drawer) {
		if (graph_draw_data_->graph_resource()) {
			if (set_polygon_flag_) {
				SetPolygon_();
				set_polygon_flag_ = false;
			}
			UpdatePolygon_();
			drawer.DrawGraph(graph_draw_data_);
		}
	}

	bool CDrawPlanet2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		_planet_component.reset(com_getter.GetComponent<CPlanet>());
		if (_planet_component == nullptr) {
			PE_LOG_ERROR("初期化に失敗しました。PlanetComponentを取得できませんでした。");
			return false;
		}
		return true;
	}

	void CDrawPlanet2D::OnInitialized() {
		Super::OnInitialized();
		SetPolygon_();
	}

	void CDrawPlanet2D::OnFinalized()noexcept {
		Super::OnFinalized();
	}

	void CDrawPlanet2D::SetPolygonRoundly_() {
		//頂点とインデックスのサイズ調整
		graph_draw_data_->SetVertexCount((_vertical_separation + 1)*(_horizontal_separation + 1));
		graph_draw_data_->SetPlygonCount((_vertical_separation - 1)*_horizontal_separation * 2 + _horizontal_separation);
		//画像と頂点の設定
		for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) { //水平方向は座標系正回りにセットしていく
			for (unsigned int j = 0; j < _vertical_separation + 1; ++j) { //垂直方向は上から順にセットしていく
				//テクスチャ座標のセット
				Vector2Df uv;
				uv.x = (float)i / _horizontal_separation;
				uv.y = (float)j / _vertical_separation;
				graph_draw_data_->SetVertexUV((_vertical_separation + 1)*i + j, uv);
			}
		}
		//インデックスの設定
		for (unsigned int i = 0; i < _horizontal_separation; ++i) {
			//中心以外はポリゴンを2枚ずつ張る
			for (unsigned int j = 0; j < _vertical_separation - 1; ++j) {
				private_::GraphDrawData2D::PolygonIndexType poly1, poly2;
				poly1[0] = i * (_vertical_separation + 1) + j;
				poly1[1] = i * (_vertical_separation + 1) + j + 1;
				poly1[2] = (i + 1)*(_vertical_separation + 1) + j;
				poly2[0] = i * (_vertical_separation + 1) + j + 1;
				poly2[1] = (i + 1)*(_vertical_separation + 1) + j;
				poly2[2] = (i + 1)*(_vertical_separation + 1) + j + 1;
				graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + j * 2, poly1);
				graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + j * 2 + 1, poly2);
			}
			//中心はポリゴンを1枚だけ張る
			private_::GraphDrawData2D::PolygonIndexType poly;
			poly[0] = i * (_vertical_separation + 1) + (_vertical_separation - 1);
			poly[1] = i * (_vertical_separation + 1) + (_vertical_separation - 1) + 1;
			poly[2] = (i + 1)*(_vertical_separation + 1) + (_vertical_separation - 1);
			graph_draw_data_->SetPolyginIndex(i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2, poly);
		}
	}

	void CDrawPlanet2D::UpdatePolygonRoundly_() {
		//今のところ拡大度は考慮していない
		Vector2Dd center_pos = GetDrawCenterPosition();
		double rotation = GetDrawRotationRed();
		//頂点座標の算出
		for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) {
			double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //現在の惑星頂点の位置角度
			double height = _planet_component->GetHeightByRad(angle_rad); //現在の惑星頂点の位置角度の高さ
			Vector2Dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //中心から地表の現在角度へのベクトル
			interface_vec *= height;
			//中心以外の頂点座標を求める
			for (unsigned int j = 0; j < _vertical_separation; ++j) {
				double dis_ratio = 1.0f - (double)j / _vertical_separation;
				graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + j, static_cast<Vector2Df>(center_pos + interface_vec * dis_ratio));
			}
			//中心の頂点座標を求める
			graph_draw_data_->SetVertexPosition(i*(_vertical_separation + 1) + _vertical_separation, static_cast<Vector2Df>(center_pos));
		}
		//頂点色設定
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->SetVertexColor(i, color());
		}
	}

	void CDrawPlanet2D::SetPolygonPlainly_() {
		//頂点とインデックスのサイズ調整
		graph_draw_data_->SetVertexCount(_horizontal_separation + 1);
		graph_draw_data_->SetPlygonCount(_horizontal_separation);
		//画像と頂点の設定
		for (unsigned int i = 0; i < _horizontal_separation; ++i) { //水平方向は座標系正回りにセットしていく
			double rad = math::PI * 2 / _horizontal_separation * i;
			//テクスチャ座標のセット
			Vector2Df uv;
			uv.x = 0.5f + (float)std::cos(rad)*0.5f;
			uv.y = 0.5f + (float)std::sin(rad)*0.5f;
			graph_draw_data_->SetVertexUV(i, uv);
		}
		graph_draw_data_->SetVertexUV(_horizontal_separation, Vector2Df(0.5f, 0.5f)); //最後の頂点は中心を指す
		//インデックスの設定
		for (unsigned int i = 0; i < _horizontal_separation; ++i) {
			//ポリゴンを1枚ずつ張る
			private_::GraphDrawData2D::PolygonIndexType poly1;
			poly1[0] = i;
			poly1[1] = (i + 1) % _horizontal_separation;
			poly1[2] = _horizontal_separation;
			graph_draw_data_->SetPolyginIndex(i, poly1);
		}
	}

	void CDrawPlanet2D::UpdatePolygonPlainly_() {
		//今のところ拡大度は考慮していない
		Vector2Dd center_pos = GetDrawCenterPosition();
		double rotation = GetDrawRotationRed();
		//頂点座標の算出
		for (unsigned int i = 0; i < _horizontal_separation; ++i) {
			double angle_rad = 2.0 * math::PI / _horizontal_separation * i; //現在の惑星頂点の位置角度
			double height = _planet_component->GetHeightByRad(angle_rad); //現在の惑星頂点の位置角度の高さ
			Vector2Dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //中心から地表の現在角度へのベクトル
			interface_vec *= height;
			graph_draw_data_->SetVertexPosition(i, static_cast<Vector2Df>(center_pos + interface_vec));
		}
		//中心頂点の設定
		graph_draw_data_->SetVertexPosition(_horizontal_separation, static_cast<Vector2Df>(center_pos));
		//頂点色設定
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->SetVertexColor(i, color());
		}
	}

	bool CDrawPlanet2D::SetGraphResource(const std::string& resource_id) {
		auto res = Game::instance().resource_manager()->GetResourceByID<RGraph>(resource_id);
		if (res == nullptr) {
			PE_LOG_ERROR("リソースの取得に失敗しました。(ResourceID: ", resource_id, ")");
			return false;
		}
		else {
			graph_draw_data_->SetGraphResource(res);
			set_polygon_flag_ = true;
			return true;
		}
	}

	CDrawPlanet2D& CDrawPlanet2D::horizontal_separation(unsigned int sep) {
		_horizontal_separation = sep;
		set_polygon_flag_ = true;
		return *this;
	}
	CDrawPlanet2D& CDrawPlanet2D::vertical_separation(unsigned int sep) {
		_vertical_separation = sep;
		set_polygon_flag_ = true;
		return *this;
	}

	void CDrawPlanet2D::texture_mapping_mode(TextureMappingMode tmm) {
		tex_map_mode_ = tmm;
		switch (tex_map_mode_) {
		case CDrawPlanet2D::TextureMappingMode::Round:
			polygon_setter_ = &CDrawPlanet2D::SetPolygonRoundly_;
			polygon_updater_ = &CDrawPlanet2D::UpdatePolygonRoundly_;
			break;
		case CDrawPlanet2D::TextureMappingMode::Plain:
			polygon_setter_ = &CDrawPlanet2D::SetPolygonPlainly_;
			polygon_updater_ = &CDrawPlanet2D::UpdatePolygonPlainly_;
			break;
		default:
			break;
		}
		set_polygon_flag_ = true;
	}

	void CDrawPlanet2D::SetPolygon_() {
		(this->*polygon_setter_)();
	}

	void CDrawPlanet2D::UpdatePolygon_() {
		(this->*polygon_updater_)();
	}

	CDrawPlanet2D& CDrawPlanet2D::graph_resource_id(const std::string& res_id) {
		SetGraphResource(res_id);
		return *this;
	}

}