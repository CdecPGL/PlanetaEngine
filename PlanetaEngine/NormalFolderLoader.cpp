#include "NormalFolderLoader.h"
#include"boost/filesystem.hpp"
#include"boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "SystemLog.h"
#include<fstream>
#include<iomanip>

namespace planeta_engine{
	namespace file_system{
		namespace bfs = boost::filesystem;

		NormalFolderLoader::NormalFolderLoader(const std::string& p) :LoaderBase(p)
		{
		}


		NormalFolderLoader::~NormalFolderLoader()
		{
		}

		bool NormalFolderLoader::_Initialize()
{
			//�t�@�C�����X�g�擾
			try{
				UpdateFileList();
				debug::SystemLog::instance().LogMessage(std::string("����������܂����B(�p�X:") + _path + "�t�@�C����:" + boost::lexical_cast<std::string>(_files.size()) + ")", "NormalFileLoader::_Initialize");
				return true;
			}
			catch (bfs::filesystem_error& e){
				debug::SystemLog::instance().LogError(std::string("�������Ɏ��s���܂����B(") + _path + ")", "NormalFileLoader::_Initialize");
				return false;
			}
		}

		void NormalFolderLoader::_Finalize() {

		}

		int NormalFolderLoader::LoadAllFileToCache(){
			int err = 0;
			for (auto& f : _files){
				if (f.second.second->GetStatus() == File::FileStatus::Available){ continue; }
				err = LoadData(f.second.second, f.second.first);
				if (err){
					debug::SystemLog::instance().LogError(std::string("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(") + f.second.first.c_str() + ")", "NormalFileLoader::LoadAllFileToCache");

				}
			}
			return err;
		}

		int NormalFolderLoader::DeleteCache(){
			for (auto& f : _files){
				f.second.second->UnloadData();
			}
			return 0;
		}

		int NormalFolderLoader::UpdateFileList(){
			DeleteCache();
			_files.clear();
			const bfs::path res_path(_path);
			BOOST_FOREACH(const bfs::path& p, std::make_pair(bfs::recursive_directory_iterator(res_path), bfs::recursive_directory_iterator())) {
				if (!bfs::is_directory(p)){
					_files.emplace(std::make_pair(p.filename().string(), std::make_pair(p.string(), std::make_shared<File>(p.extension().string()))));
				}
			}
			return 0;
		}

		std::shared_ptr<File> NormalFolderLoader::LoadFile(const std::string& fn){
			auto it = _files.find(fn);
			if (it == _files.end()){ return nullptr; } //�Ȃ�������h��
			if (it->second.second->GetStatus() == File::FileStatus::Available) { return it->second.second; } //�ǂݍ��ݍς݂�������Ԃ�
			if (LoadData(it->second.second, it->second.first)){ //�ǂݍ��ݎ��s������h��
				it->second.second->ErrorOccured();
				return nullptr;
			}
			else{ //�ǂݍ��񂾂�Ԃ�
				return it->second.second;
			}
		}

		unsigned int NormalFolderLoader::GetCacheSize()const{
			unsigned int res = 0;
			for (auto it : _files){
				res += it.second.second->GetSize();
			}
			return res;
		}

		int NormalFolderLoader::LoadData(const std::shared_ptr<File>& file, const std::string& ap){
			std::ifstream ifs(ap, std::ios::binary | std::ios::in);
			if (!ifs){ return -1; }
			//�T�C�Y�擾
			ifs.seekg(0, std::ios::end);
			int size = (int)ifs.tellg();
			//	size -= 4;
			//�������m��(File�Ɋi�[���A�Ǘ����ς˂�̂ł����ł͍폜���Ȃ�)
			unsigned char* ptr = new unsigned char[size];
			if (ptr == nullptr){
				file->ErrorOccured();
				ifs.close();
				return -1;
			}
			//�f�[�^������
			/*for (int i = 0; i < size; ++i){
				ptr[i] = 0;
				}*/
			//�ǂݍ���
			ifs.seekg(0, std::ios::beg);
			ifs.read((char*)ptr, sizeof(unsigned char)*size);

			//�f�o�b�K��������Ƃ��ēǂݍ��߂�悤��
			/*char str[16384]; int ii = 0;
			for (int i = size-16383<0 ? 0:size-16383 ; i < size; ++i,++ii){
			str[ii] = ptr[i];
			}
			str[ii] = '\0';*/

			//�f�[�^������ɓǂݍ��܂ꂽ�T�C�Y�v�Z
			/*int iii = size;
			for (; iii >= 0; --iii){
			if (ptr[iii - 1] != 0){ break; }
			}*/

			//�R���\�[���Ƀf�[�^���o��
			/*for (int i = 0; i < iii+1; ++i){
				if (i % 16 == 0){ printf("%#x : ", i); }
				printf("%02x ", ptr[i]);
				if (i % 16 == 15){ printf("\n"); }
				}
				std::cout << std::endl;*/


			file->SetData(ptr, size);
			//�I��
			ifs.close();
			return 0;
		}
	}
}
