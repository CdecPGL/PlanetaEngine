#pragma once

#include <string>
#include "planeta/core/NonCopyable.hpp"

namespace plnt {
	class IDebugInformationAdder;
	namespace private_ {
		class SubSystemManager : private util::NonCopyable<SubSystemManager>{
		public:
			virtual ~SubSystemManager() = 0 {};
			/*�f�o�b�O���ǉ��n���h��*/
			virtual void DebugInfotmationAddHandler(IDebugInformationAdder& di_adder) {};
			/*�f�o�b�O���`�����l�����쐬����*/
			void CreateDebugInformationChannel(const std::string& channel_id);
			/*�f�o�b�O���`�����l�����폜����*/
			void DeleteDebugInformationChannel();
		private:
			std::string debug_info_channel_id_;
		};
	}
}
