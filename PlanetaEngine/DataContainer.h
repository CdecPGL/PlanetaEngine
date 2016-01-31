#pragma once

#include <memory>
#include <string>

namespace planeta_engine {
	namespace utility {
		class DataContainer;
	}
}
namespace boost {
	namespace serialization {
		template<class Archive>
		void save(Archive& ar, const planeta_engine::utility::DataContainer& datacontainer, unsigned int);
		template<class Archive>
		void load(Archive& ar, planeta_engine::utility::DataContainer& data_container, unsigned int);
	}
}
namespace planeta_engine {
	namespace utility {
		class DataContainer {
		public:
			DataContainer();
			DataContainer(const DataContainer& obj) = delete;
			DataContainer(DataContainer&& obj);
			~DataContainer();

			DataContainer& operator=(const DataContainer& obj) = delete;
			DataContainer& operator=(DataContainer&& obj);

			int64_t GetInteger(const std::string& id)const;
			std::string GetString(const std::string& id)const;
			bool GetBoolean(const std::string& id)const;
			double GetNumber(const std::string& id)const;

			void SetInteger(const std::string& id, int64_t value);
			void SetString(const std::string& id, const std::string& value);
			void SetBoolean(const std::string& id, bool value);
			void SetNumber(const std::string& id, double value);
		private:
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			//boost�V���A���C�Y�̂��߂̃t�����h�w��
			template<class Archive>
			friend void boost::serialization::save(Archive& ar, const planeta_engine::utility::DataContainer& datacontainer, unsigned int);
			template<class Archive>
			friend void boost::serialization::load(Archive& ar, planeta_engine::utility::DataContainer& data_container, unsigned int);
		};
	}
}
