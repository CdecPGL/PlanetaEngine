#include <vector>
#include "boost/assign.hpp"
#include "GameDataElementType.h"


namespace planeta_engine {
	namespace core {

		std::string GameDataElementType::ConvertTypeToTypeID(Type type)noexcept
		{
			auto it = type_tid_map_.left.find(type);
			assert(it != type_tid_map_.left.end());
			return it->second;
		}

		GameDataElementType::Type GameDataElementType::ConvertTypeIDToType(const std::string type_id)noexcept
		{
			auto it = type_tid_map_.right.find(type_id);
			if (it != type_tid_map_.right.end()) { //äÓñ{å^Ç≈Ç»Ç©Ç¡ÇΩÇÁ
				if (IsTypeExist(type_id)) { //å^Ç∆ÇµÇƒë∂ç›ÇµÇƒÇ¢ÇΩÇÁï°çáå^
					return Type::complex_type;
				}
				else { //ñ≥Ç©Ç¡ÇΩÇÁÉGÉâÅ[
					return Type::nil;
				}
			}
			else {
				return it->second;
			}
		}

		void GameDataElementType::ResistComplexType(const std::string& type_id, GameDataComplexType&& complex_type)noexcept
		{
			complex_type_map_.emplace(type_id, std::move(complex_type));
		}

		const GameDataComplexType& GameDataElementType::GetComplexType(const std::string& type_id)
		{
			auto it = complex_type_map_.find(type_id);
			if (it == complex_type_map_.end()) {
				throw GameDataError(std::string("ë∂ç›ÇµÇ»Ç¢GameDataï°çáå^") + type_id + "Ç™óvãÅÇ≥ÇÍÇ‹ÇµÇΩÅB");
			}
			else {
				return it->second;
			}
		}

		bool GameDataElementType::IsTypeExist(const std::string& type_id) noexcept
		{
			return type_tid_map_.right.find(type_id) != type_tid_map_.right.end() || complex_type_map_.find(type_id) != complex_type_map_.end();
		}

		namespace {
			//éQè∆ http://stackoverflow.com/questions/20288871/how-to-construct-boost-bimap-from-static-list
			template <typename T, typename U>
			class create_bimap_unordered_set
			{
				using bimap_type = boost::bimaps::bimap< boost::bimaps::unordered_set_of<T>, boost::bimaps::unordered_set_of<U> >;
				using value_type = typename bimap_type::value_type;
			private:
				bimap_type m_map;
			public:
				create_bimap_unordered_set(const T& left, const U& right)
				{
					m_map.insert(value_type(left, right));
				}

				create_bimap_unordered_set<T, U>& operator()(const T& left, const U& right)
				{
					m_map.insert(value_type(left, right));
					return *this;
				}

				operator bimap_type()
				{
					return m_map;
				}
			};
		}

		boost::bimaps::bimap<boost::bimaps::unordered_set_of<GameDataElementType::Type>, boost::bimaps::unordered_set_of<std::string>> GameDataElementType::type_tid_map_ = create_bimap_unordered_set<GameDataElementType::Type,std::string>
			(GameDataElementType::Type::int32_type, "int32")
			(GameDataElementType::Type::int64_type, "int64")
			(GameDataElementType::Type::bool_type, "bool")
			(GameDataElementType::Type::double_type, "double")
			(GameDataElementType::Type::string_type, "string")
			(GameDataElementType::Type::array_type, "array")
			(GameDataElementType::Type::complex_type, "complex")
			(GameDataElementType::Type::nil, "nil");
	}
}