#include "GraphDrawData2D.hpp"
#include "DXGraphDrawData.hpp"
#include "DXUtility.hpp"
#include "RGraph.hpp"
#include <cassert>

namespace plnt {
	namespace private_ {
		namespace {
			/*DX形式頂点情報の初期化*/
			inline void InitializeDXVERTEX3D(VERTEX3D& dx_vtx3d) {
				dx_vtx3d.pos = VGet(0, 0, 0);
				dx_vtx3d.norm = VGet(0.0f, 0.0f, -1.0f); //画面側を向くように
				dx_vtx3d.u = 0;
				dx_vtx3d.v = 0;
				dx_vtx3d.dif = GetColorU8(255, 255, 255, 255);
			}

			/*DX形式頂点情報の位置を設定*/
			inline void SetPositionToDXVERTEX3D(VERTEX3D& dx_vtx3d, const Vector2Df& position) {
				using namespace util::dx;
				dx_vtx3d.pos = PEVector2DToDXVECTOR(position);
			}
			/*DX形式頂点情報のUV座標を設定*/
			inline void SetUVToDXVERTEX3D(VERTEX3D& dx_vtx3d, float u,float v, const Vector2Dd& image_area) {
				dx_vtx3d.u = (float)(u * image_area.x);
				dx_vtx3d.v = (float)(v * image_area.y);
			}
			/*DX形式頂点情報の色を設定*/
			inline void SetColorToDXVERTEX3D(VERTEX3D& dx_vtx3d, const Color& color) {
				using namespace util::dx;
				dx_vtx3d.dif = PEColorToDXCOLORU8(color);
//				vdx[i].spc = PEColorToDXCOLORU8(vertexes[i].color); //ライティング計算を行わないので使用しない
			}
			/*DX形式頂点情報を設定*/
			inline void SetPEVertex2DToDXVERTEX3D(VERTEX3D& dx_vtx3d, const util::Vertex2D& pe_vtx2d,const Vector2Dd& image_area) {
				using namespace util::dx;
				SetPositionToDXVERTEX3D(dx_vtx3d, pe_vtx2d.position);
				SetColorToDXVERTEX3D(dx_vtx3d, pe_vtx2d.color);
				SetUVToDXVERTEX3D(dx_vtx3d, pe_vtx2d.u, pe_vtx2d.v, image_area);
			}
			/*DX形式ポリゴンインデックス情報を設定*/
			inline void ConvertPEPolygonIndexToDXIndexArray(unsigned short* dx_idx_array_ptr,size_t write_pos_top,const std::array<size_t,3>& pe_pidx) {
				for (int j = 0; j < 3; ++j) {
					dx_idx_array_ptr[write_pos_top + j] = (unsigned short)pe_pidx[j];
				}
			}
		}

		GraphDrawData2D::GraphDrawData2D() :dx_data_(std::make_unique<DXGraphDrawData>()) {}

		GraphDrawData2D::~GraphDrawData2D() = default;

		void GraphDrawData2D::SetGraphResource(const std::shared_ptr<RGraph>& g_res)
		{
			graph_resource_ = g_res;
			dx_data_->graph_handle = g_res->GetHandle();
			//頂点UV座標の更新
			if (dx_data_->vertexes) {
				for (size_t i = 0; i < dx_data_->vertex_count; ++i) {
					auto& vtx = dx_data_->vertexes[i];
					SetUVToDXVERTEX3D(vtx, vertex_uv_information_buffer_[i].x, vertex_uv_information_buffer_[i].y, g_res->image_area());
				}
			}
		}

		void GraphDrawData2D::SetVertexCount(size_t c)
		{
			dx_data_->vertexes = std::make_unique<VERTEX3D[]>(c);
			dx_data_->vertex_count = c;
			for (size_t i = 0; i < c; ++i) {
				InitializeDXVERTEX3D(dx_data_->vertexes[i]);
			}
			vertex_uv_information_buffer_.resize(c);
		}

		void GraphDrawData2D::SetPlygonCount(size_t c)
		{
			dx_data_->indexes = std::make_unique<unsigned short[]>(c * 3);
			dx_data_->polygon_count = c;
		}

		bool GraphDrawData2D::SetVectex(size_t idx, const util::Vertex2D& vtx)
		{
			if (dx_data_->vertex_count <= idx) { return false; }
			SetPEVertex2DToDXVERTEX3D(dx_data_->vertexes[idx], vtx, graph_resource_->image_area());
			vertex_uv_information_buffer_[idx].x = vtx.u;
			vertex_uv_information_buffer_[idx].y = vtx.v;
			return true;
		}

		bool GraphDrawData2D::SetVertexPosition(size_t idx, const Vector2Df& pos)
		{
			if (dx_data_->vertex_count <= idx) { return false; }
			SetPositionToDXVERTEX3D(dx_data_->vertexes[idx], pos);
			return true;
		}

		bool GraphDrawData2D::SetVertexUV(size_t idx, const Vector2Df& uv)
		{
			if (dx_data_->vertex_count <= idx) { return false; }
			if (graph_resource_ == nullptr) { return false; }
			SetUVToDXVERTEX3D(dx_data_->vertexes[idx], uv.x, uv.y, graph_resource_->image_area());
			vertex_uv_information_buffer_[idx] = uv;
			return true;
		}

		bool GraphDrawData2D::SetVertexColor(size_t idx, const Color& color)
		{
			if (dx_data_->vertex_count <= idx) { return false; }
			SetColorToDXVERTEX3D(dx_data_->vertexes[idx], color);
			return true;
		}

		bool GraphDrawData2D::SetPolyginIndex(size_t idx, const std::array<size_t, 3>& p_idx)
		{
			if (dx_data_->polygon_count <= idx) { return false; }
			ConvertPEPolygonIndexToDXIndexArray(dx_data_->indexes.get(), idx * 3, p_idx);
			return true;
		}

		void GraphDrawData2D::SetVertexes(const std::vector<util::Vertex2D>& vertexes)
		{
			using namespace util::dx;
			//頂点情報のDX形式変換
			std::unique_ptr<VERTEX3D[]> vdx = std::make_unique<VERTEX3D[]>(vertexes.size());
			vertex_uv_information_buffer_.resize(vertexes.size());
			for (size_t i = 0; i < vertexes.size(); ++i) {
				InitializeDXVERTEX3D(vdx[i]);
				SetPEVertex2DToDXVERTEX3D(vdx[i], vertexes[i], graph_resource_->image_area());
				vertex_uv_information_buffer_[i].x = vertexes[i].u;
				vertex_uv_information_buffer_[i].y = vertexes[i].v;
			}
			dx_data_->vertexes = std::move(vdx);
			dx_data_->vertex_count = vertexes.size();
		}

		void GraphDrawData2D::SetPolygonIndexes(const std::vector<std::array<size_t, 3>>& polygon_indexes)
		{
			//インデックス情報のDX形式変換
			std::unique_ptr<unsigned short[]> idx = std::make_unique<unsigned short[]>(polygon_indexes.size() * 3);
			for (size_t i = 0; i < polygon_indexes.size(); ++i) {
				ConvertPEPolygonIndexToDXIndexArray(idx.get(), i * 3, polygon_indexes[i]);
			}
			dx_data_->indexes = std::move(idx);
			dx_data_->polygon_count = polygon_indexes.size();
		}

		void GraphDrawData2D::SetVertexesAndPolygonIndexes(const std::vector<util::Vertex2D>& vertexes, const std::vector<std::array<size_t, 3>>& indexes)
		{
			SetVertexes(vertexes);
			SetPolygonIndexes(indexes);
		}

		bool GraphDrawData2D::is_valid() const
		{
			return graph_resource_ != nullptr && dx_data_->indexes != nullptr && dx_data_->vertexes != nullptr;
		}

		size_t GraphDrawData2D::vertex_count() const
		{
			return dx_data_->vertex_count;
		}

		size_t GraphDrawData2D::polygon_count() const
		{
			return dx_data_->polygon_count;
		}

		GraphDrawData2D& GraphDrawData2D::operator=(const GraphDrawData2D& obj) {
			dx_data_->graph_handle = obj.dx_data_->graph_handle;
			dx_data_->vertex_count = obj.dx_data_->vertex_count;
			dx_data_->polygon_count = obj.dx_data_->polygon_count;
			dx_data_->vertexes = std::make_unique<VERTEX3D[]>(dx_data_->vertex_count);
			memcpy_s(dx_data_->vertexes.get(), sizeof(VERTEX3D)*dx_data_->vertex_count, obj.dx_data_->vertexes.get(), sizeof(VERTEX3D)*dx_data_->vertex_count);
			dx_data_->indexes = std::make_unique<unsigned short[]>(dx_data_->polygon_count * 3);
			memcpy_s(dx_data_->indexes.get(), sizeof(unsigned short)*dx_data_->polygon_count * 3, obj.dx_data_->indexes.get(), sizeof(unsigned short)*dx_data_->polygon_count * 3);

			graph_resource_ = obj.graph_resource_;
			vertex_uv_information_buffer_ = obj.vertex_uv_information_buffer_;
			return *this;
		}

	}
}