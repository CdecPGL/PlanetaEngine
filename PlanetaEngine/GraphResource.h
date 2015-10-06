#pragma once
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta_engine {
	namespace resources {
		/*�摜���\�[�X*/
		class GraphResource : public core::ResourceBase {
		public:
			GraphResource() :_handle(-1){};
			~GraphResource() {};
			/*�摜�n���h�����擾����B�ʏ�g�����Ƃ͂Ȃ��B*/
			int GetHandle()const { return _handle; }
			/*�摜�T�C�Y���擾*/
			const Vector2D<int>& size()const { return _image_size; }
			/*�����T�C�Y���擾�B�ʏ�g�����Ƃ͂Ȃ��B*/
			const Vector2D<int>& internal_size()const { return _internal_size; }
		private:
			int _handle;
			/*�摜�T�C�Y*/
			Vector2D<int> _image_size;
			/*�����T�C�Y*/
			Vector2D<int> _internal_size;
			bool _Create(const std::shared_ptr<file_system::File>& file) override;
			void _Dispose() override;
			bool _AdjustImageSize(); //DX���C�u������DrawPolygon�֐��ɍ��킹�āA�摜��8�ȏ��2�ׂ̂��搔�ɍ��킹��
		};
	}
}
