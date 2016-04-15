#pragma once

#include <memory>
#include <array>
#include <vector>
#include "Vertex2D.h"
#include "Color.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
	}
	namespace core {
		struct DXGraphDrawData;
		class GraphDrawData2D {
		public:
			GraphDrawData2D();
			~GraphDrawData2D();
			const DXGraphDrawData& GetDXData()const { return *dx_data_; }
			/*画像リソースをセットする*/
			void SetGraphResource(const std::shared_ptr<resources::GraphResource>& g_res);
			void SetVertexCount(size_t c); //頂点数を設定する
			void SetPlygonCount(size_t c); //ポリゴン数を設定する
			bool SetVectex(size_t idx,const utility::Vertex2D& vtx); //頂点を設定する
			bool SetVertexPosition(size_t idx,const Vector2Df& pos); //頂点の位置を設定する
			bool SetVertexUV(size_t idx, const Vector2Df& uv); //頂点のUV座標を設定する
			bool SetVertexColor(size_t idx, const Color& color); //頂点の色を設定する
			using PolygonIndexType = std::array<size_t, 3>;
			bool SetPolyginIndex(size_t idx,const std::array<size_t,3>& p_idx); //ポリゴンの頂点インデックスを設定する
			void SetVertexes(const std::vector<utility::Vertex2D>& vertexes); //頂点を全て設定
			void SetPolygonIndexes(const std::vector<std::array<size_t, 3>>& polygon_indexes); //ポリゴンインデックスを全て設定
			void SetVertexesAndPolygonIndexes(const std::vector<utility::Vertex2D>& vertexes, const std::vector<std::array<size_t, 3>>& polygon_indexes); //頂点とポリゴンインデックスをまとめて設定する
			bool is_valid()const;
			size_t vertex_count()const;
			size_t polygon_count()const;
			std::shared_ptr<resources::GraphResource> graph_resource()const { return graph_resource_; }
		private:
			std::unique_ptr<DXGraphDrawData> dx_data_;
			std::shared_ptr<resources::GraphResource> graph_resource_;
			std::vector<Vector2Df> vertex_uv_information_buffer_; //頂点UVデータの更新用に、元のUV座標データを保持しておく
		};
	}
}
