#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace planeta_engine {
	namespace core {
		/*メンバ変数データ基底クラス*/
		template<class C>
		class MemberVariableDataBase {
		public:
			MemberVariableDataBase() = default;
			virtual ~MemberVariableDataBase() = default;

		};
		/*メンバ変数データクラス*/
		template<class C,typename T>
		class MemberVariableData final: public MemberVariableDataBase<C> {
		public:
			MemberVariableData(T C::* mvp):member_variable_pointer_(mvp) {}
		private:
			T C::* member_variable_pointer_;
		};
		/*プロパティデータクラス*/
		template<class C, typename T>
		class PropertyData1 final : public MemberVariableDataBase<C> {
		public:
			using GetterType = const T(C::*)();
			using SetterType = void(C::*)(T);
			/*ReadWrite*/
			PropertyData1(GetterType getter, SetterType setter) :member_variable_pointer_(mvp) : getter_(getter), setter_(setter) {}
			/*ReadOnly*/
			PropertyData1(GetterType getter) :member_variable_pointer_(mvp) : getter_(getter), setter_(nullptr) {}
			/*WriteOnly*/
			PropertyData1(SetterType setter) :member_variable_pointer_(mvp) : getter_(nullptr), setter_(setter) {}
		private:
			GetterType getter_;
			SetterType setter_;
		};
		template<class C, typename T>
		class PropertyData2 final : public MemberVariableDataBase<C> {
		public:
			using GetterType = const T&(C::*)();
			using SetterType = void(C::*)(const T&);
			/*ReadWrite*/
			PropertyData2(GetterType getter, SetterType setter) :member_variable_pointer_(mvp) : getter_(getter), setter_(setter) {}
			/*ReadOnly*/
			PropertyData2(GetterType getter) :member_variable_pointer_(mvp) : getter_(getter), setter_(nullptr) {}
			/*WriteOnly*/
			PropertyData2(SetterType setter) :member_variable_pointer_(mvp) : getter_(nullptr), setter_(setter) {}
		private:
			GetterType getter_;
			SetterType setter_;
		};
		/*メンバ関数データ基底クラス*/
		template<class C>
		class MemberFunctionDataBase {
		public:
			MemberFunctionDataBase() = default;
			virtual ~MemberFunctionDataBase() = default;
		};
		/*メンバ関数データクラス*/
		template<class C,typename Ret,typename... Args>
		class MemberFunctionData final : public MemberFunctionDataBase<C> {
		public:
			MemberFunctionData(Ret(C::*mfp)(Args...)):member_function_pointer_(mfp) {}
		private:
			Ret(C::*member_function_pointer_)(Args...);
		};
		/*クラスデータ基底クラス*/
		class ClassDataBase {

		};
		template<class C>ClassDataResister;
		/*クラスデータクラス*/
		template<class C>
		class ClassData final : public ClassDataBase {
			friend ClassDataResister<C>;
		private:
			std::unordered_map<std::string, std::unique_ptr<MemberVariableDataBase<C>>> variable_;
			std::unordered_map<std::string, std::unique_ptr<MemberFunctionDataBase<C>>> function_;
		};

		template<class C>
		class ClassDataResister {
		public:
			ClassDataResister():class_data_(std::make_unique<ClassData<C>>()) {};
			~ClassDataResister() = default;
			template<typename T>
			ClassDataResister& Variable(const std::string& identifer,T C::*member_veriable_pointer) {
				class_data_->variable_.emplace(identifer, std::make_unique<MemberVariableData<C, T>>(member_veriable_pointer));
				return *this;
			}
			ClassDataResister& Property(const std::string& identifer, T C::*member_veriable_pointer) {
				class_data_->variable_.emplace(identifer, std::make_unique<MemberVariableData<C, T>>(member_veriable_pointer));
				return *this;
			}
			template<typename Ret,typename... Args>
			ClassDataResister& Function(const std::string& identifer, Ret(C::* member_function_pointer)(Args...)){
				class_data_->function_.emplace(identifer, std::make_unique<MemberFunctionData<C, Ret, Args...>>(member_function_pointer));
				return *this;
			}
		private:
			std::unique_ptr<ClassData<C>> class_data_;
		};



	}
}

