#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <typeindex>
#include "boost/lexical_cast.hpp"
#include "MetaprogrammingUtility.h"
#include "SystemLog.h"

/*������*/
namespace planeta {
	namespace util {
		class Reflection {
		public:
			Reflection() = default;
			virtual ~Reflection() = default;
			template<typename ... TArgs>
			bool Call(const std::string& func_name, TArgs ... params) {
				//�q�N���X���珇�ɏ������Ă����B���s�ɐ���������I���B
				for (auto it = function_map_.rbegin(); it!= function_map_.rend(); ++it) {
					auto fit = it->second->find(func_name);
					if (fit = it->second->end()) {
						continue;
					}
					else {
						if (SearchFunction(fit->second, params...)) { return true; }
						else { continue; }
					}
				}
				debug::SystemLog::instance().LogWarning(std::string("�֐������݂��܂���B") + func_name + +"(" + mp_utiliey::ConvertParametersToString(params...) + ")", __FUNCTION__);
				return false;
			}
		protected:
			/*�K��Reflection���g���N���X�̃R���X�g���N�^�S�ĂŎ��s����*/
			template<class C>
			void SetUpRefrection(C* this_pointer) {
				static ClassInformationSetupper re(this_pointer); //�N���X�̓o�^�����B�ÓI�֐��Ȃ̂ŏ��񂵂����s����Ȃ��B
				auto it = type_function_map_.find(typeid(C));
				assert(it != type_function_map_.end()); //�N���X�o�^�͍ς�ł���͂��Ȃ̂Ō�����Ȃ��̂͂��������B
				if (function_map_.size() > 0 && (*(function_map_.end() - 1)) == &(it->second)) { //�֐��}�b�v�̍Ō�����A�V���ɓo�^���悤�Ƃ��Ă�����̂ƈ�v����
					debug::SystemLog::instance().LogWarning(std::string("���t���N�V�����̃Z�b�g�A�b�v���d�����čs���܂����B(ClassType = ") + typeid(C).name() + ", Address = " + boost::lexical_cast<std::string>(this_pointer) + ")", __FUNCTION__);
					return;
				}
				function_map_.push_back(&it->second);
			}			

		private:
			struct FunctionHolder {
				std::unordered_map<std::type_index, boost::any> functions;
			};
			using FunctionMap = std::unordered_map<std::string, FunctionHolder>;
		protected:
			class RefrectionInformationResistInterface final {
			public:
				RefrectionInformationResistInterface(FunctionMap& func_map,const std::function<bool(Reflection*)>& type_checker) :func_map_(func_map),type_checker_(type_checker) {};
				template<typename ClassType, typename ... TArgs>
				void ResistFunction(const std::string name, const std::function<void(ClassType*, TArgs ...)>& func) {
					//�L���ȃ^�C�v���m�F
					if (!type_checker_(&ClassType())) {
						throw std::logic_error(std::string("�N���X�̃^�C�v���s���ł��B") + name + "(" + mp_utiliey::ConvertParameterTypesToString<TArgs...>() + ")");
					}
					//�o�^����֐����쐬
					std::function<void(Reflection* this_base, TArgs...)> resist_func = [func](Reflection* this_base, TArgs... params) {func(static_cast<ClassType*>(this_base), params...)};
					//�֐��o�^���ɗp����^�C�v�f�[�^
					using namespace mp_utiliey;
					using resist_func_type = typename ExpandParameterList < typename ConvertParameterList<typename List<Reflection*, TArgs...>::list>::param_list>::type;
					std::type_info resist_func_type_info = typeid(resist_func);
					/*void(Refrection*,Params...)�̌`���Ŋ֐���o�^*/
					func_map_[name].functions.emplace(resist_func_type_info, resist_func);
				}
				void ResistToClassFactory(const std::string&){

				}
			private:
				Reflection::FunctionMap& func_map_;
				std::function<bool(Reflection*)> type_checker_;
			};
		private:

			virtual void ResistRefrectionInformation(RefrectionInformationResistInterface&&) = 0;

			std::vector<FunctionMap*> function_map_;
			static std::unordered_map<std::type_index, FunctionMap> type_function_map_;

			template<typename ... Params>
			bool SearchFunction(const FunctionHolder& fh, Params ... params) {
				using func_type = std::function<void(Params...)>;
				std::type_info func_type_info = typeid(func_type);
				auto it = fh.functions.find(func_type_info);
				if (it == fh.functions.end()) {
					return false;
				}
				else {
					CallFunction(it->second, this, params...);
				}
			}

			template<typename ... TArgs>
			void CallFunction(const boost::any& func,TArgs ... params) {
				using namespace mp_utiliey;
				using func_type_temp = typename ExpandParameterList < typename ConvertParameterList<typename List<TArgs...>::list>::param_list>::type;
				try {
					func_type_temp::Call(boost::any_cast<typename func_type_temp::func_type<void>>(func), params...);
				}
				catch (boost::bad_any_cast&) {
					assert(false); //�֐��^�͓o�^���ƌ������ň�v���Ă���͂��Ȃ̂ŁA�����ŕs��v�Ȃ̂͂��������B
				}
			}

			class ClassInformationSetupper final{
			public:
				template<class C>
				explicit ClassInformationSetupper(C* c) {
					Reflection* this_r = c;
					auto it = this_r->type_function_map_.emplace(typeid(C), FunctionMap()).first; //�N���X����o�^
					//�������Ăяo����鎞�_��C�N���X���\�z����Ă���͂��Ȃ̂ő��v�H
					this_r->ResistRefrectionInformation(RefrectionInformationResistInterface(it->second, [](Reflection* r) {return dynamic_cast<C*>(r) ? true : false; }));
				}
				~ClassInformationSetupper() = default;
			};
		};
	}
}