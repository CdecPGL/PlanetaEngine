#pragma once

#include "ResourceBase.h"

namespace planeta {
	class RPtree final: public private_::ResourceBase {
	public:
		std::shared_ptr<const boost::property_tree::ptree> GetPtree()const;
	private:
		bool _Create(const File& file) override;
		void _Dispose() override;
		std::shared_ptr<boost::property_tree::ptree> ptree_;
	};
}