#pragma once

#include <memory>
#include <string>

namespace planeta {
	class File;
	namespace util {
		/*�W���t�@�C�����͂���File���쐬����*/
		std::shared_ptr<File> CreateFileFromStandardFileInput(const std::string& file_name);
		/*���s�R�[�h��\n�ɓ��ꂷ��(\r\n��\r��\n��)*/
		void UnifyLineFeedCode(std::string& str);
		/*���s�R�[�h����菜��(\r��\n)*/
		void RemoveLineFeedCode(std::string& str);
		/*�X�y�[�X�ƃ^�u����菜��*/
		void RemoveSpaceAndTab(std::string& str);
	}
}