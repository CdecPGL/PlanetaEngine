#pragma once

#include "ResourceBase.hpp"

namespace planeta {
	/*!Boost::Ptree形式で読み込まれるリソース*/
	class RPtree : public ResourceBase {
	public:
		virtual ~RPtree() = default;
		std::shared_ptr<const boost::property_tree::ptree> GetPtree()const;
	private:
		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed() override;
		std::shared_ptr<boost::property_tree::ptree> ptree_;
	};
}