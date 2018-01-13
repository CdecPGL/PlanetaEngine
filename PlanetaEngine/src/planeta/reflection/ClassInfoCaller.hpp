#pragma once

#include <string>
#include <memory>

#include "boost/any.hpp"
#include "boost/property_tree/ptree.hpp"

namespace plnt::reflection {
	class Reflectable;
	namespace private_ {
		struct ClassInfo;
		class ClassInfoCaller {
		public:
			ClassInfoCaller(const ClassInfo& class_info)noexcept;
			void SetVariable(const std::string& var_id, Reflectable& obj, const boost::any& v)const;
			void GetVariable(const std::string& var_id, Reflectable& obj, boost::any& v)const;
			void SetDataFromPtree(const boost::property_tree::ptree& pt, Reflectable& obj);
			std::shared_ptr<Reflectable> Clone(Reflectable& src_obj);
			void CopyFrom(Reflectable& me, const Reflectable&src);
		private:
			const ClassInfo& class_info_;
		};
	}
}
