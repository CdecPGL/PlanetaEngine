#pragma once

#include "planeta/core/GameObjectSystemComponent.hpp"
#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class CTransform2D;

	/*! 二次元カメラコンポーネント*/
	class CCamera2D final : public private_::GameObjectSystemComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CCamera2D);

	public:
		using Super = private_::GameObjectSystemComponent;

		CCamera2D();
		~CCamera2D();
		/*! 位置を取得*/
		const plnt::Vector2Dd &position() const;
		/*! 位置を設定*/
		void position(const plnt::Vector2Dd &p);
		/*! 回転度を取得*/
		double rotation_rad() const;
		/*! 回転度を設定*/
		void rotation_rad(double r);
		/*! 拡大度を取得*/
		double expansion() const;
		/*! 拡大度を設定*/
		void expansion(double e);

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;

		double expansion_ = 1.0;

		NonOwingPointer<CTransform2D> transform2d_;
		virtual bool GetOtherComponentsProc(const GOComponentGetter &com_getter) override;
		virtual void OnInitialized() override;
		virtual void OnActivated() override;
		virtual void OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CCamera2D);
}
