#include "GameDataElement.h"
#include "GameDataElementType.h"

namespace planeta_engine {
	namespace core {
		class GameDataElement::Impl_ {
		public:
			Impl_(const std::string& type_id,int) :type_(GameDataElementType::ConvertTypeIDToType(type_id)) {
				assert(type_ != GameDataElementType::Type::nil);
				switch (type_)
				{
				case GameDataElementType::Type::string_type: //stringの場合は新たに領域確保
					data_.string_data = new std::string();
					break;
				case GameDataElementType::Type::array_type: //arrayの場合は新たに領域確保
					data_.array_data = new std::vector<GameDataElement>();
					break;
				case GameDataElementType::Type::complex_type: //ComplexTypeの場合は新たに領域確保
					data_.complex_data = nullptr;
					try {
						const GameDataComplexType& complex_type = GameDataElementType::GetComplexType(type_id);
						data_.complex_data = new GameDataComplexTypeInstance(complex_type);
						complex_type_id_ = data_.complex_data->complex_type().type_id();
					}
					catch (GameDataError&) { //型の存在チェックは外で済ませる
						if (data_.complex_data) { data_.complex_data; } //?
						assert(false);
					}
					break;
				default: //それ以外は何もしない
					break;
				}
			}
			Impl_(const Impl_& obj):type_(obj.type_),complex_type_id_(obj.complex_type_id_) {
				switch (type_)
				{
				case GameDataElementType::Type::string_type: //stringの場合はnewコピーコンストラクタで領域を確保
					data_.string_data = new std::string(*data_.string_data);
					break;
				case GameDataElementType::Type::array_type: //arrayの場合はnewコピーコンストラクタで領域を確保
					data_.array_data = new std::vector<GameDataElement>(*data_.array_data);
					break;
				case GameDataElementType::Type::complex_type: //ComplexTypeの場合はnewコピーコンストラクタで領域を確保
					data_.complex_data = new GameDataComplexTypeInstance(*data_.complex_data);
					break;
				default: //その他の場合は値をコピー
					data_ = obj.data_;
					break;
				}
			}
			Impl_(Impl_&&) = delete;
			Impl_(int32_t v) :type_(GameDataElementType::Type::int32_type) { data_.int32_data = v; }
			Impl_(int64_t v) :type_(GameDataElementType::Type::int64_type) { data_.int64_data = v; }
			Impl_(bool v) :type_(GameDataElementType::Type::bool_type) { data_.bool_data = v; }
			Impl_(double v) :type_(GameDataElementType::Type::double_type) { data_.double_data = v; }
			Impl_(const std::string& v) :type_(GameDataElementType::Type::string_type) { data_.string_data = new std::string(v); }
			Impl_(std::string&& v) :type_(GameDataElementType::Type::string_type) { data_.string_data = new std::string(std::move(v)); }
			Impl_(const std::vector<GameDataElement>& v) :type_(GameDataElementType::Type::array_type) { data_.array_data = new std::vector<GameDataElement>(v); }
			Impl_(std::vector<GameDataElement>&& v) :type_(GameDataElementType::Type::array_type) { data_.array_data = new std::vector<GameDataElement>(std::move(v)); }
			Impl_(const GameDataComplexTypeInstance& v) :type_(GameDataElementType::Type::complex_type) {
				data_.complex_data = new GameDataComplexTypeInstance(v);
				complex_type_id_ = data_.complex_data->complex_type().type_id();
			}
			Impl_(GameDataComplexTypeInstance&& v) :type_(GameDataElementType::Type::complex_type) {
				data_.complex_data = new GameDataComplexTypeInstance(std::move(v));
				complex_type_id_ = data_.complex_data->complex_type().type_id();
			}
			~Impl_() {
				Dispose();
			}
			Impl_& operator=(const Impl_& obj) = delete;
			Impl_& operator=(Impl_&&) = delete;
			GameDataElementType::Type type_; //型
			std::string complex_type_id_ = GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil); //複合型の場合の型ID
			union data
			{
				int32_t int32_data;
				int64_t int64_data;
				bool bool_data;
				double double_data;
				//以下はサイズが大きいのでポインタで保持
				std::string* string_data;
				std::vector<GameDataElement>* array_data;
				GameDataComplexTypeInstance* complex_data;
			};
			data data_;
			void TypeCheck(GameDataElementType::Type t)const{
				if (type_ != t) { throw GameDataError(std::string("GameDataElementの型が不正です。(自分の型:" + GameDataElementType::ConvertTypeToTypeID(type_) + ",入力型:" + GameDataElementType::ConvertTypeToTypeID(t) + ")")); }
			}
			void ComplexTypeCheck(const std::string& complex_type_id)  {
				if (complex_type_id_ != complex_type_id) { throw GameDataError(std::string("GameDataElementの型が不正です。複合型の型が一致しません。(自分の型:") + complex_type_id_ + ",入力型:" + complex_type_id + ")"); }
			}
		private:
			void Dispose() {
				switch (type_)
				{
				case GameDataElementType::Type::string_type: //stringは確保してある領域を破棄
					delete data_.string_data;
					break;
				case GameDataElementType::Type::array_type: //arrayは確保してある領域を破棄
					delete data_.array_data;
					break;
				case GameDataElementType::Type::complex_type: //ComplexTypeは確保してある領域を破棄
					delete data_.complex_data;
					break;
				default:
					break;
				}
			}
		};

		GameDataElement GameDataElement::CreateByTypeID(const std::string& type_id) {
			if (!GameDataElementType::IsTypeExist(type_id)) {
				throw GameDataError(std::string("存在しないGameDataElement型")+type_id+"が要求されました。");
			}
			return std::move(GameDataElement(type_id));
		}

		GameDataElement::GameDataElement() = default;

		GameDataElement::GameDataElement(const std::string& type_id,int)noexcept:impl_(std::make_unique<Impl_>(type_id,0))
		{
		}

		GameDataElement::GameDataElement(const GameDataElement& element)noexcept : impl_(std::make_unique<Impl_>(*element.impl_))
		{
		}

		GameDataElement::GameDataElement(GameDataElement&& element)noexcept : impl_(std::move(element.impl_))
		{
		}

		GameDataElement::GameDataElement(int32_t v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(int64_t v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(bool v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(double v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(const std::string& v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(std::string&& v)noexcept : impl_(std::make_unique<Impl_>(std::move(v))) {}
		GameDataElement::GameDataElement(const std::vector<GameDataElement>& v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(std::vector<GameDataElement>&& v)noexcept : impl_(std::make_unique<Impl_>(std::move(v))) {}
		GameDataElement::GameDataElement(const GameDataComplexTypeInstance& v)noexcept : impl_(std::make_unique<Impl_>(v)) {}
		GameDataElement::GameDataElement(GameDataComplexTypeInstance&& v)noexcept : impl_(std::make_unique<Impl_>(std::move(v))) {}

		GameDataElement::~GameDataElement()noexcept = default;

		GameDataElement& GameDataElement::operator=(const GameDataElement& element)noexcept {
			impl_ = std::make_unique<Impl_>(*element.impl_);
			return *this;
		}

		GameDataElement& GameDataElement::operator=(GameDataElement&& element)noexcept {
			impl_ = std::move(element.impl_);
			return *this;
		}

		const std::string GameDataElement::type_id()const noexcept {
			if (impl_ == nullptr) { return GameDataElementType::ConvertTypeToTypeID(GameDataElementType::Type::nil); }
			if (impl_->type_ == GameDataElementType::Type::complex_type) {
				return impl_->complex_type_id_;
			}
			else {
				return GameDataElementType::ConvertTypeToTypeID(impl_->type_);
			}
		}

		//値を取得
		const int32_t GameDataElement::GetInt32()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::int32_type);
			return impl_->data_.int32_data;
		}
		const int64_t GameDataElement::GetInt64()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::int64_type);
			return impl_->data_.int64_data;
		}
		const bool GameDataElement::GetBool()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::bool_type);
			return impl_->data_.bool_data;
		}
		const double GameDataElement::GetDouble()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::double_type);
			return impl_->data_.double_data;
		}
		const std::string& GameDataElement::GetString()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::string_type);
			return *(impl_->data_.string_data);
		}
		const std::vector<GameDataElement>& GameDataElement::GetArray()const {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::array_type);
			return *(impl_->data_.array_data);
		}
		std::vector<GameDataElement>& GameDataElement::GetArrayRef() {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::array_type);
			return *(impl_->data_.array_data);
		}
		const GameDataComplexTypeInstance& GameDataElement::GetComplexType()const  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::complex_type);
			return *(impl_->data_.complex_data);
		}
		GameDataComplexTypeInstance& GameDataElement::GetComplexTypeRef()  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::complex_type);
			return *(impl_->data_.complex_data);
		}
		//値を設定
		void GameDataElement::SetInt32(int32_t v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::int32_type);
			impl_->data_.int32_data = v;
		}
		void GameDataElement::SetInt64(int64_t v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::int64_type);
			impl_->data_.int64_data = v;
		}
		void GameDataElement::SetBool(bool v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::bool_type);
			impl_->data_.bool_data = v;
		}
		void GameDataElement::SetDouble(double v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::double_type);
			impl_->data_.double_data = v;
		}
		void GameDataElement::SetString(const std::string& v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::string_type);
			*(impl_->data_.string_data) = v;
		}
		void GameDataElement::SetString(std::string&& v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::string_type);
			*(impl_->data_.string_data) = std::move(v);
		}
		void GameDataElement::SetArray(const std::vector<GameDataElement>& v) {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::array_type);
			*(impl_->data_.array_data) = v;
		}
		void GameDataElement::SetArray(std::vector<GameDataElement>&& v) {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::array_type);
			*(impl_->data_.array_data) = std::move(v);
		}
		void GameDataElement::SetComplexType(const GameDataComplexTypeInstance& v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::complex_type);
			impl_->ComplexTypeCheck(v.complex_type().type_id());
			delete impl_->data_.complex_data;
			impl_->data_.complex_data = new GameDataComplexTypeInstance(v);
		}
		void GameDataElement::SetComplexType(GameDataComplexTypeInstance&& v)  {
			if (impl_ == nullptr) { throw GameDataError("空のGameDataelementにアクセスしようとしました。"); }
			impl_->TypeCheck(GameDataElementType::Type::complex_type);
			impl_->ComplexTypeCheck(v.complex_type().type_id());
			delete impl_->data_.complex_data;
			impl_->data_.complex_data = new GameDataComplexTypeInstance(std::move(v));
		}

		void GameDataElement::SetType_for_boost_serialize_(const std::string& type_id)
		{
			if(!GameDataElementType::IsTypeExist(type_id)){
				throw GameDataError(std::string("存在しないGameDataElement型") + type_id + "が要求されました。");
			}
			impl_ = std::make_unique<Impl_>(type_id, 0);
		}

	}
}