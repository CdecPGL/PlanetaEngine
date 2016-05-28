#include "DebugManager.h"
#include "SystemLog.h"
#include "DxLib.h"
#include <windows.h>

namespace {
	constexpr char* WINDOW_NAME("DebugWindow");

	LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		/*switch (uMsg) {

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}*/
	}
}

namespace planeta_engine {
	namespace debug {

		//////////////////////////////////////////////////////////////////////////
		//Impl
		//////////////////////////////////////////////////////////////////////////

		class DebugManager::Impl_ {
		public:
			HWND debug_window_handle;
		};

		//////////////////////////////////////////////////////////////////////////
		//DebugInformationDisplayer
		//////////////////////////////////////////////////////////////////////////

		DebugManager::DebugManager() :impl_(std::make_unique<Impl_>()) {}
		DebugManager::~DebugManager() = default;

		bool DebugManager::Initialize() {
			PE_LOG_WARNING("DebugManagerは未実装どす。");
			return true;
			//HWND m_hWnd = GetMainWindowHandle();
			//HINSTANCE hInst = GetModuleHandle(NULL);

			//WNDCLASSEX wc;

			//wc.cbClsExtra = 0;////////////////////////////////////////////////////  とりあえず0を指定すればOK
			//wc.cbSize = sizeof(WNDCLASSEX);//                                           構造体のサイズ
			//wc.cbWndExtra = 0;///////////////////////////////////////////////////   とりあえず0を指定すればOK
			//wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);//        背景色を指定。
			//wc.hCursor = LoadCursor(hInst, IDC_ARROW);// マウスカーソルの指定。とりあえずこれを指定しておけばOK
			//wc.hIcon = NULL;//                                              ウインドウの左上に表示されるアイコンの指定。とりあえずNULLでOK
			//wc.hIconSm = NULL;///////////////////////////////   とりあえずNULLでOK
			//wc.hInstance = hInst;//                                     WinMainの第1引数HINSTANCEに指定したものを指定する
			//wc.lpfnWndProc = DebugWindowProc;///////////////  ｳｲﾝﾄﾞｳﾌﾟﾛｼｰｼﾞｬ
			//wc.lpszClassName = WINDOW_NAME;//             ｳｲﾝﾄﾞｳｸﾗｽ名を指定。CreateWindowの第1引数に指定する
			//wc.lpszMenuName = NULL;//////////////////////// ｳｲﾝﾄﾞｳﾒﾆｭｰﾘｿｰｽを示す文字列か識別子を指定。とりあえずNULLでOK
			//wc.style = CS_DBLCLKS;//                                    ウインドウのスタイルを指定

			//if (RegisterClassEx(&wc) == 0) {
			//	SystemLog::instance().LogError("デバッグウインドウクラスの登録に失敗しました。", __FUNCTION__);
			//	return false;
			//}

			//HWND nhwnd = CreateWindow(WINDOW_NAME, "デバッグ用情報", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 640, 480, m_hWnd, NULL, hInst, NULL);
			//if (nhwnd == NULL) {
			//	SystemLog::instance().LogError("デバッグ情報ウインドウの作成に失敗しました。", __FUNCTION__);
			//	return false;
			//}
			//SystemLog::instance().LogMessage("デバッグ情報ウインドウを作成しました。", __FUNCTION__);
			//if (ShowWindow(nhwnd, SW_SHOWNA)) {
			//	SystemLog::instance().LogWarning("デバッグ情報ウインドウはすでに表示されていました。", __FUNCTION__);
			//} else {
			//	SystemLog::instance().LogMessage("デバッグ情報ウインドウを表示しました。", __FUNCTION__);
			//}
			//UpdateWindow(nhwnd);
			//impl_->debug_window_handle = nhwnd;

			//return true;
		}

		void DebugManager::Finalize() {
			//DestroyWindow(impl_->debug_window_handle);
		}

		void DebugManager::Update() {
			HDC hdc = GetDC(impl_->debug_window_handle);
			SetWindowText(impl_->debug_window_handle, "test");
			ReleaseDC(impl_->debug_window_handle,hdc);
		}

	}
}