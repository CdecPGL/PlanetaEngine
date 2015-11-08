#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "GameDataComplexTypeInstance.h"
#include "GameDataError.h"

namespace planeta_engine {
	namespace core {
		class GameDataElement;
	}
}
namespace boost {
	namespace serialization {
		template<class Archive>
		void load(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int);
	}
}
namespace planeta_engine{
	namespace core {
		class GameDataElement final{
		public:
			/*型IDからエレメントを作成*/
			static GameDataElement CreateByTypeID(const std::string& type_id);
			/*コピーコンストラクタ*/
			GameDataElement(const GameDataElement& element)noexcept;
			/*ムーブコンストラクタ*/
			GameDataElement(GameDataElement&& element)noexcept;
			/*値設定用コンストラクタ群(暗黙的変換を有効にするためexplicitはつけない)*/
			GameDataElement(int32_t v)noexcept;
			GameDataElement(int64_t v)noexcept;
			GameDataElement(bool v)noexcept;
			GameDataElement(double v)noexcept;
			GameDataElement(const std::string& v)noexcept;
			GameDataElement(std::string&& v)noexcept;
			GameDataElement(const std::vector<GameDataElement>& v)noexcept;
			GameDataElement(std::vector<GameDataElement>&& v)noexcept;
			GameDataElement(const GameDataComplexTypeInstance& v)noexcept;
			GameDataElement(GameDataComplexTypeInstance&& v)noexcept;
			/*代入演算子*/
			GameDataElement& operator=(const GameDataElement& element)noexcept;
			/*ムーブ演算子*/
			GameDataElement& operator=(GameDataElement&& element)noexcept;
			/*デストラクタ*/
			~GameDataElement()noexcept;
			//型を取得
			const std::string type_id()const noexcept;
			//値を取得
			const int32_t GetInt32()const;
			const int64_t GetInt64()const;
			const bool GetBool()const;
			const double GetDouble()const;
			const std::string& GetString()const;
			const std::vector<GameDataElement>& GetArray()const;
			std::vector<GameDataElement>& GetArrayRef();
			const GameDataComplexTypeInstance& GetComplexType()const;
			GameDataComplexTypeInstance& GetComplexTypeRef();
			//値を設定
			void SetInt32(int32_t v);
			void SetInt64(int64_t v);
			void SetBool(bool v);
			void SetDouble(double v);
			void SetString(const std::string& v);
			void SetString(std::string&& v);
			void SetArray(const std::vector<GameDataElement>& v);
			void SetArray(std::vector<GameDataElement>&& v);
			void SetComplexType(const GameDataComplexTypeInstance& v);
			void SetComplexType(GameDataComplexTypeInstance&& v);
		private:
			/*通常の型設定コンストラクタ(文字列設定コンストラクタと区別するためのダミーとしてint型を第二引数に取る)*/
			GameDataElement(const std::string& type_id, int)noexcept;
			class Impl_;
			std::unique_ptr<Impl_> impl_;
			//boostシリアライズのためのフレンド指定
			template<class Archive>
			friend void boost::serialization::load<Archive>(Archive& ar, planeta_engine::core::GameDataElement& game_data_element, unsigned int);
			//boostシリアライズ用
			void SetType_for_boost_serialize_(const std::string& type_id);
		};
	}
}
