#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "GameObject.h"

#include "TLSFMemoryAllocator.h"
//#include "GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace planeta_engine::core;

namespace PlanetaEngineTest
{		
	class Hoge{
		char data[sizeof(std::vector<int>)];
	};
	using TestType = Hoge;
	const int test_count=100000;

	TEST_CLASS(TLSFMemoryAllocatorTest)
	{
	public:
		TLSFMemoryAllocator tlsf;
		TLSFMemoryAllocatorTest(){
			tlsf.ReserveMemory(1024 * 1024 * 16);
		};
		TEST_METHOD(�ʏ�new_delete����){
			TestType* ptrs[test_count];
			for (int i = 0; i < test_count; ++i){
				ptrs[i] = new TestType;
			}
			for (int i = 0; i < test_count; ++i){
				delete ptrs[i];
			}
		}

		TEST_METHOD(TLSFmalloc_dealloc����){
			using namespace planeta_engine::core;
			TestType* ptrs[test_count];
			for (int i = 0; i < test_count; ++i){
				ptrs[i] = tlsf.Alloc<TestType>();
			}
			for (int i = 0; i < test_count; ++i){
				tlsf.Dealloc(ptrs[i]);
			}
		}
		
		TEST_METHOD(TLSLMemoryAllocator�e�X�g)
		{
			// TODO: �e�X�g �R�[�h�������ɑ}�����܂�
			using namespace planeta_engine::core;
			try{
				TLSFMemoryAllocator tlsl;
				TLSFMemoryAllocator::MemoryDetailScanResult scres;
				Assert::IsTrue(tlsl.ReserveMemory(1024*1024), L"�������̈�m�ۂɎ��s!");
				scres = tlsl.DetailScan();
				int *test = tlsl.Alloc<int>();
				*test = 12345;
				scres = tlsl.DetailScan();
				std::vector<unsigned long long*> tests;
				Assert::IsNotNull(test, L"�������̊��蓖�Ď��s!!");
				for (int i = 0; i < 30000; ++i){
					unsigned long long* t = tlsl.Alloc<unsigned long long>();
					tests.push_back(t);
					*t = (unsigned)(123456 + i);
				}
				scres = tlsl.DetailScan();
				Assert::AreEqual(sizeof(unsigned long long) * 30000 + sizeof(int), tlsl.GetAllocatedMemorySize(), L"�T�C�Y�̕s��v!!!");
				Assert::AreEqual(30000u + 1u, tlsl.GetAllocatedMemoryCount(), L"���̕s��v!!!!");
				Assert::AreEqual(12345, *test, L"�f�[�^�j��");
				try{
					tlsl.Dealloc(test);
					scres = tlsl.DetailScan();
				}
				catch (BadAlloc&){
					Assert::Fail(L"�������̉���Ɏ��s");
				}
				bool error = false;
				try{ tlsl.Dealloc(test); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"����ς݃|�C���^�̌��o���s");
				error = false;
				try{ tlsl.Dealloc(nullptr); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"�͈͊O�|�C���^�̌��o���s");
				error = false;
				try{ tlsl.Dealloc(reinterpret_cast<char*>(test)+1); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"�����|�C���^�̌��o���s");

				scres = tlsl.DetailScan();
				Assert::IsFalse(scres.error, L"�G���[�X�L�����ŃG���[���o");
			}
			catch (BadAlloc&){
				Assert::Fail(L"�Ȃɂ������ς������B");
			}
		}

	};
}