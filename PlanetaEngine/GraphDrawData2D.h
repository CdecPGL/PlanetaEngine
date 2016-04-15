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
			/*�摜���\�[�X���Z�b�g����*/
			void SetGraphResource(const std::shared_ptr<resources::GraphResource>& g_res);
			void SetVertexCount(size_t c); //���_����ݒ肷��
			void SetPlygonCount(size_t c); //�|���S������ݒ肷��
			bool SetVectex(size_t idx,const utility::Vertex2D& vtx); //���_��ݒ肷��
			bool SetVertexPosition(size_t idx,const Vector2Df& pos); //���_�̈ʒu��ݒ肷��
			bool SetVertexUV(size_t idx, const Vector2Df& uv); //���_��UV���W��ݒ肷��
			bool SetVertexColor(size_t idx, const Color& color); //���_�̐F��ݒ肷��
			using PolygonIndexType = std::array<size_t, 3>;
			bool SetPolyginIndex(size_t idx,const std::array<size_t,3>& p_idx); //�|���S���̒��_�C���f�b�N�X��ݒ肷��
			void SetVertexes(const std::vector<utility::Vertex2D>& vertexes); //���_��S�Đݒ�
			void SetPolygonIndexes(const std::vector<std::array<size_t, 3>>& polygon_indexes); //�|���S���C���f�b�N�X��S�Đݒ�
			void SetVertexesAndPolygonIndexes(const std::vector<utility::Vertex2D>& vertexes, const std::vector<std::array<size_t, 3>>& polygon_indexes); //���_�ƃ|���S���C���f�b�N�X���܂Ƃ߂Đݒ肷��
			bool is_valid()const;
			size_t vertex_count()const;
			size_t polygon_count()const;
			std::shared_ptr<resources::GraphResource> graph_resource()const { return graph_resource_; }
		private:
			std::unique_ptr<DXGraphDrawData> dx_data_;
			std::shared_ptr<resources::GraphResource> graph_resource_;
			std::vector<Vector2Df> vertex_uv_information_buffer_; //���_UV�f�[�^�̍X�V�p�ɁA����UV���W�f�[�^��ێ����Ă���
		};
	}
}
