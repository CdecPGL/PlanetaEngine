#pragma once

#include "..\core\game_object_standard_component.hpp"
#include "planeta/core/NonOwingPointer.hpp"

namespace plnt {
	class CEffect final : public game_object_standard_component {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CEffect);

	public:
		using super = game_object_standard_component;
		CEffect();
		~CEffect();
		/*! エフェクトを再生する*/
		bool Play();
		/*! エフェクトを停止する*/
		bool Stop();
		/*! エフェクトリソースIDを設定する*/
		void resource_id(const std::string &resource_id);
		/*! エフェクトを再生中か確認*/
		bool is_playing() const;
		/*! ループするかのフラグを設定*/
		void roop_flag(bool f);
		/*! ループするかのフラグを取得*/
		bool roop_flag() const;
		/*! 自動再生するかどうかのフラグを設定*/
		void auto_play(bool f);
		/*! 自動再生するかどうかのフラグを取得*/
		bool auto_play() const;
		/*! エフェクトの拡大率を設定*/
		void expansion(double e);
		/*! エフェクトの拡大率を取得*/
		double expansion() const;

	protected:
		virtual bool get_other_components_proc(const go_component_getter &) override;
		virtual void on_initialized() override;
		virtual void on_activated() override;
		virtual void on_inactivated() override;
		virtual void on_finalized() noexcept override;

	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CEffect);
}
