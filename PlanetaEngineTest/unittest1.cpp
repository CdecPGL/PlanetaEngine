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
		TEST_METHOD(通常new_delete時間){
			TestType* ptrs[test_count];
			for (int i = 0; i < test_count; ++i){
				ptrs[i] = new TestType;
			}
			for (int i = 0; i < test_count; ++i){
				delete ptrs[i];
			}
		}

		TEST_METHOD(TLSFmalloc_dealloc時間){
			using namespace planeta_engine::core;
			TestType* ptrs[test_count];
			for (int i = 0; i < test_count; ++i){
				ptrs[i] = tlsf.Alloc<TestType>();
			}
			for (int i = 0; i < test_count; ++i){
				tlsf.Dealloc(ptrs[i]);
			}
		}
		
		TEST_METHOD(TLSLMemoryAllocatorテスト)
		{
			// TODO: テスト コードをここに挿入します
			using namespace planeta_engine::core;
			try{
				TLSFMemoryAllocator tlsl;
				TLSFMemoryAllocator::MemoryDetailScanResult scres;
				Assert::IsTrue(tlsl.ReserveMemory(1024*1024), L"メモリ領域確保に失敗!");
				scres = tlsl.DetailScan();
				int *test = tlsl.Alloc<int>();
				*test = 12345;
				scres = tlsl.DetailScan();
				std::vector<unsigned long long*> tests;
				Assert::IsNotNull(test, L"メモリの割り当て失敗!!");
				for (int i = 0; i < 30000; ++i){
					unsigned long long* t = tlsl.Alloc<unsigned long long>();
					tests.push_back(t);
					*t = (unsigned)(123456 + i);
				}
				scres = tlsl.DetailScan();
				Assert::AreEqual(sizeof(unsigned long long) * 30000 + sizeof(int), tlsl.GetAllocatedMemorySize(), L"サイズの不一致!!!");
				Assert::AreEqual(30000u + 1u, tlsl.GetAllocatedMemoryCount(), L"個数の不一致!!!!");
				Assert::AreEqual(12345, *test, L"データ破損");
				try{
					tlsl.Dealloc(test);
					scres = tlsl.DetailScan();
				}
				catch (BadAlloc&){
					Assert::Fail(L"メモリの解放に失敗");
				}
				bool error = false;
				try{ tlsl.Dealloc(test); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"解放済みポインタの検出失敗");
				error = false;
				try{ tlsl.Dealloc(nullptr); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"範囲外ポインタの検出失敗");
				error = false;
				try{ tlsl.Dealloc(reinterpret_cast<char*>(test)+1); }
				catch (BadAlloc&){ error = true; }
				Assert::IsTrue(error, L"無効ポインタの検出失敗");

				scres = tlsl.DetailScan();
				Assert::IsFalse(scres.error, L"エラースキャンでエラー検出");
			}
			catch (BadAlloc&){
				Assert::Fail(L"なにかしっぱいした。");
			}
		}

	};
}