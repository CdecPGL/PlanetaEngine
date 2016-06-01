#pragma once
#include "Object.h"
#include <unordered_map>

namespace planeta {
	namespace util {
		class ParameterHolder final: public core::Object{
		public:
			/*�t���O��ݒ�(���O�A�l)*/
			bool SetFlag(const std::string& name, bool value) { _flag_map[name] = value; }
			/*�t���O���擾(���O�A�f�t�H���g�l(���݂��Ȃ������ꍇ�̒l))*/
			bool GetFlag(const std::string& name, bool default_value) {
				try {
					return _flag_map.at(name);
				}
				catch (std::out_of_range&) {
					return default_value;
				}
			}
			/*������ݒ�(���O�A�l)*/
			void SetInteger(const std::string& name, int value) { _integer_map[name] = value; }
			/*�������擾(���O�A�f�t�H���g�l(���݂��Ȃ������ꍇ�̒l))*/
			int GetInteger(const std::string& name, int default_value) {
				try {
					return _integer_map.at(name);
				}
				catch (std::out_of_range&) {
					return default_value;
				}
			}
			/*������ݒ�(���O�A�l)*/
			void SetNumber(const std::string& name, double value) { _number_map[name] = value; }
			/*�������擾(���O�A�f�t�H���g�l(���݂��Ȃ������ꍇ�̒l))*/
			double GetNumbder(const std::string& name, double default_value) {
				try {
					return _number_map.at(name);
				}
				catch (std::out_of_range&) {
					return default_value;
				}
			}
			/*�������ݒ�(���O�A�l)*/
			void SetString(const std::string& name, const std::string& value) { _string_map[name] = value; }
			/*��������擾(���O�A�f�t�H���g�l(���݂��Ȃ������ꍇ�̒l))*/
			const std::string GetString(const std::string& name, const std::string& default_value) {
				try {
					return _string_map.at(name);
				}
				catch (std::out_of_range&) {
					return default_value;
				}
			}
			/*���ׂẴp�����[�^���폜*/
			void ClearAllParameters() {
				_flag_map.clear();
				_integer_map.clear();
				_number_map.clear();
				_string_map.clear();
			}
		private:
			std::unordered_map<std::string, bool> _flag_map;
			std::unordered_map<std::string, int> _integer_map;
			std::unordered_map<std::string, double> _number_map;
			std::unordered_map<std::string, std::string> _string_map;
		};
	}
}
