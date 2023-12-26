#pragma once

#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	/*効果音リソース*/
	class RSound : public ResourceBase {
	public:
		RSound() : _handle(-1) { };

		~RSound() { };
		int GetHandle() const { return _handle; }
		int GetTotalTimeByMilliSecond();

	private:
		int _handle;
		virtual bool OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) override;
		virtual void OnDisposed() override;
	};
}
