#pragma once

#include "ResourceBase.h"

namespace planeta {
	class RPtree final: public ResourceBase {
	public:
		std::shared_ptr<const boost::property_tree::ptree> GetPtree()const;
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		std::shared_ptr<boost::property_tree::ptree> ptree_;
	};
}