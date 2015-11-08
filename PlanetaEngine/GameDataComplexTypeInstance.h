#pragma once

#include <unordered_map>
#include "GameDataComplexType.h"
#include "GameDataError.h"

namespace planeta_engine {
	namespace core {
		class GameDataComplexTypeInstance;
	}
}
namespace boost {
	namespace serialization {
		template<class Archive>
		void save(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_element, unsigned int);
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_element, unsigned int);
	}
}

namespace planeta_engine {
	namespace core {
		class GameDataElement;
		class GameDataComplexTypeInstance {
		public:
			GameDataComplexTypeInstance(const GameDataComplexTypeInstance& obj)noexcept:complex_type_(obj.complex_type_),elements_(obj.elements_) {}
			GameDataComplexTypeInstance(GameDataComplexTypeInstance&& obj)noexcept :complex_type_(obj.complex_type_), elements_(std::move(obj.elements_)) {}
			explicit GameDataComplexTypeInstance(const GameDataComplexType& t)noexcept :complex_type_(&t){}
			const GameDataElement& GetElement(const std::string& element_id)const;
			void SetElement(const std::string& element_id, const GameDataElement& element);
			void SetElement(const std::string& element_id, GameDataElement&& element);
			const GameDataComplexType& complex_type()const noexcept{ return *complex_type_; }
		private:
			const GameDataComplexType* complex_type_;
			mutable std::unordered_map<std::string, GameDataElement> elements_;
			//boostシリアライズのためのフレンド指定
			template<class Archive>
			friend void boost::serialization::save<Archive>(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_element, unsigned int);
			template<class Archive>
			friend void boost::serialization::load<Archive>(Archive& ar, planeta_engine::core::GameDataComplexTypeInstance& game_data_element, unsigned int);
			//boostシリアライズ用
			void SetType_for_boost_serialize_(const GameDataComplexType& complex_type) { complex_type_ = &complex_type; }
		};
	}
}