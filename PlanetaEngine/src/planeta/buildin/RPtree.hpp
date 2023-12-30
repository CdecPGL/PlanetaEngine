#pragma once

#include "planeta/core/ResourceBase.hpp"

namespace plnt {
	/*!Boost::Ptree形式で読み込まれるリソース*/
	class RPtree : public resource_base {
	public:
		virtual ~RPtree() = default;
		std::shared_ptr<const boost::property_tree::ptree> GetPtree() const;

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::shared_ptr<boost::property_tree::ptree> ptree_;
	};
}
