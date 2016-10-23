#pragma once

#include "GameObjectStandardComponent.h"
#include "NonOwingPointer.h"

namespace planeta {
	class CEffect final: public GameObjectStandardComponent {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CEffect);
	public:
		using Super = GameObjectStandardComponent;
		CEffect();
		~CEffect();
		void resource_id(const std::string& resource_id);
		bool is_playing()const;
		void roop_flag(bool f);
		bool roop_flag()const;
	protected:
		virtual bool GetOtherComponentsProc(const GOComponentGetter&) override;
		virtual bool OnInitialized() override;
		virtual bool OnActivated() override;
		virtual bool OnInactivated() override;
		virtual void OnFinalized() noexcept override;
	private:
		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CEffect);
}