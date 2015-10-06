#include "DrawPlanetComponent.h"
#include <cmath>
#include "DrawManager.h"
#include "ResourceManager.h"
#include "GraphResource.h"
#include "IGameObjectAccessor.h"
#include "PlanetComponent.h"
#include "MathConstant.h"
#include "SystemLog.h"

namespace {
	constexpr unsigned int kDefaultHorizontalSeparation(90);
	constexpr unsigned int kDefaultVerticalSeparation(5);
}

namespace planeta_engine {
	namespace components {

		DrawPlanetComponent::DrawPlanetComponent() :_horizontal_separation(kDefaultHorizontalSeparation), _vertical_separation(kDefaultVerticalSeparation)
		{

		}

		void DrawPlanetComponent::Draw()
		{
			_UpdatePolygon();
			core::DrawManager::instance().DrawGraph(_vertexes, _indexes, _graph_resource);
		}

		bool DrawPlanetComponent::Initialize_()
		{
			if (DrawComponent::Initialize_() == false) { return false; }
			_planet_component = game_object()->GetComponent<components::PlanetComponent>();
			if (_planet_component) {
				_SetPolygon();
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("初期化に失敗しました。PlanetComponentを取得できませんでした。", "DrawPlanetComponent::Initialize_");
				return false;
			}
		}

		void DrawPlanetComponent::Finalize_()
		{
			DrawComponent::Finalize_();
		}

		void DrawPlanetComponent::_SetPolygon()
		{
			//頂点とインデックスのサイズ調整
			_vertexes.resize((_vertical_separation + 1)*(_horizontal_separation + 1));
			_indexes.resize((_vertical_separation - 1)*_horizontal_separation * 2 + _horizontal_separation);
			//画像と頂点の設定
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) { //水平方向は座標系正回りにセットしていく
				for (unsigned int j = 0; j < _vertical_separation + 1; ++j) { //垂直方向は上から順にセットしていく
					//テクスチャ座標のセット
					_vertexes[(_vertical_separation + 1)*i + j].u = (float)i / _horizontal_separation;
					_vertexes[(_vertical_separation + 1)*i + j].v = (float)j / _vertical_separation;
				}
			}
			//インデックスの設定
			for (unsigned int i = 0; i < _horizontal_separation; ++i) {
				//中心以外はポリゴンを2枚ずつ張る
				for (unsigned int j = 0; j < _vertical_separation - 1; ++j) {
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][0] = i*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][1] = i*(_vertical_separation + 1) + j + 1;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2][2] = (i + 1)*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][0] = i*(_vertical_separation + 1) + j + 1;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][1] = (i + 1)*(_vertical_separation + 1) + j;
					_indexes[i*(_vertical_separation * 2 - 1) + j * 2 + 1][2] = (i + 1)*(_vertical_separation + 1) + j + 1;
				}
				//中心はポリゴンを1枚だけ張る
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][0] = i*(_vertical_separation + 1) + (_vertical_separation - 1);
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][1] = i*(_vertical_separation + 1) + (_vertical_separation - 1) + 1;
				_indexes[i*(_vertical_separation * 2 - 1) + (_vertical_separation - 1) * 2][2] = (i + 1)*(_vertical_separation + 1) + (_vertical_separation - 1);
			}
		}

		void DrawPlanetComponent::_UpdatePolygon()
		{
			if (game_object() == nullptr || _graph_resource == nullptr || _planet_component == nullptr) { return; }
			//今のところ拡大度は考慮していない
			Vector2D<double> center_pos = GetDrawCenterPosition();
			double rotation = GetDrawRotationRed();
			//頂点座標の算出
			for (unsigned int i = 0; i < _horizontal_separation + 1; ++i) {
				double angle_rad = 2.0 * math::constant::PI / _horizontal_separation * i; //現在の惑星頂点の位置角度
				double height = _planet_component->GetHeightByRad(angle_rad); //現在の惑星頂点の位置角度の高さ
				Vector2D<double> interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation)); //中心から地表の現在角度へのベクトル
				interface_vec *= height;
				//中心以外の頂点座標を求める
				for (unsigned int j = 0; j < _vertical_separation; ++j) {
					double dis_ratio = 1.0f - (double)j / _vertical_separation;
					_vertexes[i*(_vertical_separation + 1) + j].position = center_pos + interface_vec*dis_ratio;
				}
				//中心の頂点座標を求める
				_vertexes[i*(_vertical_separation + 1) + _vertical_separation].position = center_pos;
			}
			//頂点色設定
			for (auto& vtx : _vertexes) {
				vtx.color = color();
			}
		}

		bool DrawPlanetComponent::SetGraphResource(const std::string& resource_id)
		{
			auto res = core::ResourceManager::instance().GetResource(resource_id);
			if (res == nullptr) {
				debug::SystemLog::instance().LogError(std::string("リソースの取得に失敗しました。(リソース名は") + resource_id + ")", "DrawPlanetComponent::SetGraphResource");
				return false;
			}
			std::shared_ptr<resources::GraphResource> gr = std::dynamic_pointer_cast<resources::GraphResource>(res);
			if (gr) {
				_graph_resource = gr;
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("画像リソースでないリソースが指定されました。(リソース名は") + resource_id + "、タイプは" + res->GetType().name() + ")", "DrawPlanetComponent::SetGraphResource");
				return false;
			}
		}

		void DrawPlanetComponent::horizontal_separation(unsigned int sep)
		{
			_horizontal_separation = sep;
			_SetPolygon();
		}
		void DrawPlanetComponent::vertical_separation(unsigned int sep)
		{
			_vertical_separation = sep;
			_SetPolygon();
		}
	}
}