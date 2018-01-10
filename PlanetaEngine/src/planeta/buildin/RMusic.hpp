#pragma once
#include "planeta/ResourceBase.hpp"

namespace plnt {
	/*音楽リソース*/
	class RMusic : public ResourceBase {
	public:
		RMusic() :_handle(-1) {};
		~RMusic() {}
		int GetHandle()const { return _handle; }
		int GetTotalTimeByMilliSecond();
	private:
		int _handle;
		virtual bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		virtual void OnDisposed() override;
	};
}

