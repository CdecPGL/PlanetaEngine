#pragma once
#include "ResourceBase.h"
#include "Vector2D.h"

namespace planeta {
	/*�摜���\�[�X*/
	class GraphResource : public core::ResourceBase {
	public:
		GraphResource() :_handle(-1) {};
		~GraphResource() {};
		/*�摜�n���h�����擾����B�ʏ�g�����Ƃ͂Ȃ��B*/
		int GetHandle()const { return _handle; }
		/*�摜�T�C�Y���擾*/
		const Vector2Di& size()const { return image_size_; }
		/*�����T�C�Y���擾�B�ʏ�g�����Ƃ͂Ȃ��B*/
		const Vector2Di& internal_size()const { return internal_size_; }
		/*�摜�T�C�Y�́A�����T�C�Y�ɑ΂��銄�����擾����B�ʏ�g�����Ƃ͂Ȃ��B*/
		const Vector2Dd& image_area()const { return image_area_; }
	private:
		int _handle;
		/*�摜�T�C�Y*/
		Vector2Di image_size_;
		/*�����T�C�Y*/
		Vector2Di internal_size_;
		/*�摜�T�C�Y/�����T�C�Y*/
		Vector2Dd image_area_;
		bool _Create(const std::shared_ptr<const File>& file) override;
		void _Dispose() override;
		bool _AdjustImageSize(); //DX���C�u������DrawPolygon�֐��ɍ��킹�āA�摜��8�ȏ��2�ׂ̂��搔�ɍ��킹��
	};
}
