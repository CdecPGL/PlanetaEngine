#pragma once

#include "../core/resource_base.hpp"

namespace plnt {
	/*!Boost::Ptree形式で読み込まれるリソース*/
	class r_ptree : public resource_base {
	public:
		[[nodiscard]] std::shared_ptr<const boost::property_tree::ptree> get_ptree() const;

	private:
		bool on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) override;
		void on_disposed() override;
		std::shared_ptr<boost::property_tree::ptree> ptree_;
	};
}
