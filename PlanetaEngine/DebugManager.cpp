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
			PE_LOG_WARNING("DebugManager�͖������ǂ��B");
			return true;
			//HWND m_hWnd = GetMainWindowHandle();
			//HINSTANCE hInst = GetModuleHandle(NULL);

			//WNDCLASSEX wc;

			//wc.cbClsExtra = 0;////////////////////////////////////////////////////  �Ƃ肠����0���w�肷���OK
			//wc.cbSize = sizeof(WNDCLASSEX);//                                           �\���̂̃T�C�Y
			//wc.cbWndExtra = 0;///////////////////////////////////////////////////   �Ƃ肠����0���w�肷���OK
			//wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);//        �w�i�F���w��B
			//wc.hCursor = LoadCursor(hInst, IDC_ARROW);// �}�E�X�J�[�\���̎w��B�Ƃ肠����������w�肵�Ă�����OK
			//wc.hIcon = NULL;//                                              �E�C���h�E�̍���ɕ\�������A�C�R���̎w��B�Ƃ肠����NULL��OK
			//wc.hIconSm = NULL;///////////////////////////////   �Ƃ肠����NULL��OK
			//wc.hInstance = hInst;//                                     WinMain�̑�1����HINSTANCE�Ɏw�肵�����̂��w�肷��
			//wc.lpfnWndProc = DebugWindowProc;///////////////  ����޳��ۼ��ެ
			//wc.lpszClassName = WINDOW_NAME;//             ����޳�׽�����w��BCreateWindow�̑�1�����Ɏw�肷��
			//wc.lpszMenuName = NULL;//////////////////////// ����޳�ƭ�ؿ�������������񂩎��ʎq���w��B�Ƃ肠����NULL��OK
			//wc.style = CS_DBLCLKS;//                                    �E�C���h�E�̃X�^�C�����w��

			//if (RegisterClassEx(&wc) == 0) {
			//	SystemLog::instance().LogError("�f�o�b�O�E�C���h�E�N���X�̓o�^�Ɏ��s���܂����B", __FUNCTION__);
			//	return false;
			//}

			//HWND nhwnd = CreateWindow(WINDOW_NAME, "�f�o�b�O�p���", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 640, 480, m_hWnd, NULL, hInst, NULL);
			//if (nhwnd == NULL) {
			//	SystemLog::instance().LogError("�f�o�b�O���E�C���h�E�̍쐬�Ɏ��s���܂����B", __FUNCTION__);
			//	return false;
			//}
			//SystemLog::instance().LogMessage("�f�o�b�O���E�C���h�E���쐬���܂����B", __FUNCTION__);
			//if (ShowWindow(nhwnd, SW_SHOWNA)) {
			//	SystemLog::instance().LogWarning("�f�o�b�O���E�C���h�E�͂��łɕ\������Ă��܂����B", __FUNCTION__);
			//} else {
			//	SystemLog::instance().LogMessage("�f�o�b�O���E�C���h�E��\�����܂����B", __FUNCTION__);
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