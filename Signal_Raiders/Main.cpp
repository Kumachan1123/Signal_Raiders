/*
*	@file Main.cpp
*	@brief �Q�[���̉�ʂ�\�����郁�C���֐�
*/
// �w�b�_�[�t�@�C��
#include <pch.h>
#include "Game.h"
#include <Libraries/MyLib/MemoryLeakDetector.h>

using namespace DirectX;

#ifdef __clang__// clang�R���p�C�����g�p���Ă���ꍇ
#pragma clang diagnostic ignored "-Wcovered-switch-default"// �f�t�H���g�̃P�[�X���J�o�[����Ă��Ȃ��Ƃ����x���𖳎�
#pragma clang diagnostic ignored "-Wswitch-enum"// �񋓌^��switch���Ɋւ���x���𖳎�
#endif
#pragma warning(disable : 4061)// enum�l��switch���ŃJ�o�[����Ă��Ȃ��Ƃ����x���𖳎�
// �E�B���h�E�X�^�C���̒�`�i�^�C�g���o�[�A�V�X�e�����j���[�A�ŏ����{�^���A�ő剻�{�^�����܂ށj
#define WS_MY_WINDOW    (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

namespace
{
	std::unique_ptr<Game> g_game;// �Q�[���I�u�W�F�N�g�̃O���[�o���|�C���^�[
}
// �A�v���P�[�V������
LPCWSTR g_szAppName = L"�V�O�i�����C�_�[�Y";
// Windows�v���V�[�W���̐錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// �Q�[���I�������̐錾
void ExitGame() noexcept;

// NVIDIA��AMD�̃O���t�B�b�N�J�[�h�̍œK���ݒ�
extern "C"// C���ꃊ���P�[�W���g�p���ăG�N�X�|�[�g���邽�߂̐錾
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;// NVIDIA Optimus��L���ɂ��邽�߂̃t���O
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // AMD�̍����\���[�h��v�����邽�߂̃t���O
}

// �A�v���̃G���g���[�|�C���g�iWinMain�j
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(_DEBUG)// �f�o�b�O�r���h�̏ꍇ
	mylib::MemoryLeakDetector();// ���������[�N���o���������
#endif
	UNREFERENCED_PARAMETER(hPrevInstance);//���g�p�x����\��
	UNREFERENCED_PARAMETER(lpCmdLine);// ���g�p�x����\��
	if (!XMVerifyCPUSupport())return 1;// CPU��DirectX Math���C�u�������T�|�[�g���Ă��Ȃ��ꍇ�͏I��
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);// COM���C�u������������(�}���`�X���b�h�x�[�X�̏��������g�p)
	if (FAILED(hr))	return 1;// COM�̏������Ɏ��s�����ꍇ�͏I��
	g_game = std::make_unique<Game>();// �Q�[���I�u�W�F�N�g���쐬
	static bool s_fullscreen = true;// �t���X�N���[�����[�h�̏����l��ݒ�(true: �t���X�N���[���Afalse: �E�B���h�E���[�h)
#ifdef _DEBUG// �f�o�b�O�r���h�̏ꍇ
	// ��ʃ��[�h�I��
	if (MessageBox(NULL, L"�t���X�N���[���ɂ��܂����H", L"��ʃ��[�h�ݒ�", MB_YESNO) == IDYES)// ���[�U�[���u�͂��v��I�������ꍇ
		s_fullscreen = true;// �t���X�N���[�����[�h�ɐݒ�
	else	// ���[�U�[���u�������v��I�������ꍇ
		s_fullscreen = false;// �E�B���h�E���[�h�ɐݒ�
#endif
	// �E�B���h�E�N���X��o�^���A�E�B���h�E���쐬����
	{

		WNDCLASSEXW wcex = {};// Windows�̃E�B���h�E�N���X���`����\����
		wcex.cbSize = sizeof(WNDCLASSEXW);// �\���̂̃T�C�Y��ݒ�
		wcex.style = CS_HREDRAW | CS_VREDRAW; // �E�B���h�E�̃X�^�C����ݒ�i��������ѐ����̃��T�C�Y���ɍĕ`��j
		wcex.lpfnWndProc = WndProc; // �E�B���h�E�v���V�[�W���̊֐��|�C���^��ݒ�
		wcex.hInstance = hInstance; // �A�v���P�[�V�����̃C���X�^���X�n���h����ݒ�
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON"); // �A�v���P�[�V�����̃A�C�R�������[�h
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW); // �f�t�H���g�̃J�[�\����ݒ�
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); // �E�B���h�E�̔w�i�F��ݒ�
		wcex.lpszClassName = L"_3DPGWindowClass";// �E�B���h�E�N���X����ݒ�
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON"); // �������A�C�R�������[�h
		if (!RegisterClassExW(&wcex)) 	return 1;// �E�B���h�E�N���X�̓o�^�Ɏ��s�����ꍇ�͏I��
		// �E�B���h�E�̏�����
		int w, h;
		if (s_fullscreen == true)// �t���X�N���[���̏ꍇ
		{
			w = 1920;// ����1920�ɐݒ�
			h = 1080; // ������1080�ɐݒ�
		}
		else g_game->GetDefaultSize(w, h);// �E�B���h�E���[�h�̏ꍇ�̓Q�[���̃f�t�H���g�T�C�Y���擾
		RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };// �E�B���h�E�̃N���C�A���g�̈�̋�`���`
		AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);// �E�B���h�E�̃N���C�A���g�̈�̋�`�𒲐�
		HWND hwnd = CreateWindowExW(// �E�B���h�E���쐬
			0,						// �g���X�^�C���Ȃ�
			L"_3DPGWindowClass",	// �E�B���h�E�N���X��
			g_szAppName,			// �A�v���P�[�V������
			WS_MY_WINDOW,			// �E�B���h�E�X�^�C���i�^�C�g���o�[�A�V�X�e�����j���[�A�ŏ����{�^���A�ő剻�{�^�����܂ށj
			CW_USEDEFAULT,			// �E�B���h�E�ʒu�i�f�t�H���g�ʒu�j
			CW_USEDEFAULT,			// �E�B���h�E�ʒu�i�f�t�H���g�ʒu�j
			rc.right - rc.left,		// �E�B���h�E�̕��i������̋�`�̕��j
			rc.bottom - rc.top,		// �E�B���h�E�̍����i������̋�`�̍����j
			nullptr,				// �e�E�B���h�E�Ȃ�
			nullptr,				// ���j���[�n���h���Ȃ�
			hInstance,				// �A�v���P�[�V�����̃C���X�^���X�n���h��
			nullptr					// �A�v���P�[�V�����̃p�����[�^�Ȃ�
		);
		if (!hwnd)	return 1;// �E�B���h�E�̍쐬�Ɏ��s�����ꍇ�͏I��
		ShowWindow(hwnd, nCmdShow);// �E�B���h�E��\������
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));// �E�B���h�E�̃��[�U�[�f�[�^�ɃQ�[���I�u�W�F�N�g�̃|�C���^��ݒ�
		GetClientRect(hwnd, &rc);// �E�B���h�E�̃N���C�A���g�̈�̋�`���擾
		g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);// �Q�[���̏��������s��
		if (s_fullscreen) g_game->SetFullscreenState(TRUE);// �t���X�N���[�����[�h�̏ꍇ�A�Q�[���̃t���X�N���[����Ԃ�ݒ�
	}
	MSG msg = {};// ���b�Z�[�W�\���̂�������
	while (WM_QUIT != msg.message)// ���b�Z�[�W��WM_QUIT�łȂ����胋�[�v�𑱂���
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))// ���b�Z�[�W���L���[�ɂ��邩���m�F���A����Ύ��o���ď���
		{
			TranslateMessage(&msg);// ���b�Z�[�W��ϊ�
			DispatchMessage(&msg); // ���b�Z�[�W���f�B�X�p�b�`
		}
		else
			g_game->Tick();		// ���b�Z�[�W���Ȃ��ꍇ�̓Q�[����Tick�֐����Ăяo���čX�V�������s��
	}
	if (s_fullscreen) g_game->SetFullscreenState(FALSE);// �t���X�N���[�����[�h�̏ꍇ�A�Q�[���̃t���X�N���[����Ԃ�����
	g_game.reset(); // �Q�[���I�u�W�F�N�g�����Z�b�g���ă����������
	CoUninitialize(); // COM���C�u�����̏I������
	return static_cast<int>(msg.wParam);// ���b�Z�[�W��wParam��Ԃ��i�A�v���P�[�V�����̏I���R�[�h�j
}
/*
*	@brief �E�B���h�E�v���V�[�W��
*	@details �E�B���h�E�ɑ����郁�b�Z�[�W����������֐�
*	@param hWnd �E�B���h�E�n���h��
*	@param message ���b�Z�[�W�̎��ʎq
*	@param wParam ���b�Z�[�W�Ɋ֘A����ǉ����
*	@param lParam ���b�Z�[�W�Ɋ֘A����ǉ����
*	@return ���b�Z�[�W�̏�������
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ��Ԃ�ێ����邽�߂̐ÓI�ϐ�
	static bool s_in_sizemove = false;// �E�B���h�E�̃T�C�Y�ύX�����ǂ����������t���O
	static bool s_in_suspend = false; // �A�v���P�[�V�������T�X�y���h��Ԃ��ǂ����������t���O
	static bool s_minimized = false;  // �A�v���P�[�V�������ŏ�������Ă��邩�ǂ����������t���O
	static bool s_fullscreen = false; // �A�v���P�[�V�������t���X�N���[�����[�h���ǂ����������t���O

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));// �E�B���h�E�̃��[�U�[�f�[�^����Q�[���I�u�W�F�N�g�̃|�C���^���擾

	switch (message)// ���b�Z�[�W�̎�ނɉ����ď����𕪊�
	{
	case WM_PAINT:// �E�B���h�E�̍ĕ`��v��
		if (s_in_sizemove && game)game->Tick();// �T�C�Y�ύX���͎��O�ŕ`��
		else								   // �T�C�Y�ύX���łȂ��ꍇ�� 
		{
			PAINTSTRUCT ps;	// �`��̂��߂̍\���̂�������
			std::ignore = BeginPaint(hWnd, &ps); // �E�B���h�E�̕`����J�n
			EndPaint(hWnd, &ps); // �`����I��
		}
		break;// switch���̏I��
	case WM_DISPLAYCHANGE:// �f�B�X�v���C�̐ݒ肪�ύX���ꂽ�Ƃ�
		if (game)game->OnDisplayChange();// �Q�[���I�u�W�F�N�g�Ƀf�B�X�v���C�ύX��ʒm
		break;// switch���̏I��
	case WM_MOVE:// �E�B���h�E���ړ����ꂽ�Ƃ�
		if (game)game->OnWindowMoved();// �Q�[���I�u�W�F�N�g�ɃE�B���h�E�ړ���ʒm
		break;// switch���̏I��
	case WM_ENTERSIZEMOVE:// �E�B���h�E�̃T�C�Y�ύX���J�n���ꂽ�Ƃ�
		s_in_sizemove = true;// �T�C�Y�ύX���t���O��ݒ�
		break;// switch���̏I��
	case WM_EXITSIZEMOVE:// �E�B���h�E�̃T�C�Y�ύX���I�������Ƃ�
		s_in_sizemove = false;// �T�C�Y�ύX���t���O������
		if (game)// �Q�[���I�u�W�F�N�g�����݂���ꍇ
		{
			RECT rc;// �E�B���h�E�̃N���C�A���g�̈�̋�`���`
			GetClientRect(hWnd, &rc);// �E�B���h�E�̃N���C�A���g�̈�̋�`���擾
			game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);// �Q�[���I�u�W�F�N�g�ɃE�B���h�E�T�C�Y�ύX��ʒm
		}
		break;// switch���̏I��
	case WM_GETMINMAXINFO:// �E�B���h�E�̍ŏ�����эő�T�C�Y���擾���邽�߂̃��b�Z�[�W
		if (lParam)// lParam��nullptr�łȂ��ꍇ
		{
			auto info = reinterpret_cast<MINMAXINFO*>(lParam); // lParam��MINMAXINFO�\���̂ɃL���X�g
			info->ptMinTrackSize.x = 320; // �ŏ��g���b�N�T�C�Y�̕���320�ɐݒ�
			info->ptMinTrackSize.y = 180; // �ŏ��g���b�N�T�C�Y�̍�����180�ɐݒ�
		}
		break;// switch���̏I��
	case WM_ACTIVATEAPP:
		if (game)// �Q�[���I�u�W�F�N�g�����݂���ꍇ
		{
			if (wParam)game->OnActivated();	// �A�v���P�[�V�������A�N�e�B�u�ɂȂ����ꍇ��OnActivated���Ăяo��
			else game->OnDeactivated();		// �A�v���P�[�V��������A�N�e�B�u�ɂȂ����ꍇ��OnDeactivated���Ăяo��
		}
		Keyboard::ProcessMessage(message, wParam, lParam);			// �L�[�{�[�h�̃��b�Z�[�W������
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);	// �}�E�X�̃��b�Z�[�W������
		break;// switch���̏I��
	case WM_POWERBROADCAST: // �V�X�e���̓d����Ԃ��ύX���ꂽ�Ƃ��ɑ��M����郁�b�Z�[�W
		switch (wParam) // wParam�̒l�ɉ����ď����𕪊�
		{
		case PBT_APMQUERYSUSPEND:// �V�X�e�����T�X�y���h��Ԃɓ���O�ɑ��M����郁�b�Z�[�W
			if (!s_in_suspend && game)	game->OnSuspending();// �Q�[���I�u�W�F�N�g�ɃT�X�y���h��ʒm
			s_in_suspend = true; // �T�X�y���h��ԃt���O��ݒ�
			return TRUE; // ���b�Z�[�W�̏����������������Ƃ�����TRUE��Ԃ�
		case PBT_APMRESUMESUSPEND: // �V�X�e�����T�X�y���h��Ԃ��畜�A�����Ƃ��ɑ��M����郁�b�Z�[�W
			if (!s_minimized)// �E�B���h�E���ŏ�������Ă��Ȃ��ꍇ
			{
				if (s_in_suspend && game)game->OnResuming();// �Q�[���I�u�W�F�N�g�ɕ��A��ʒm
				s_in_suspend = false;// �T�X�y���h��ԃt���O������
			}
			return TRUE;// ���b�Z�[�W�̏����������������Ƃ�����TRUE��Ԃ�
		}
		break;// switch���̏I��
	case WM_DESTROY:// �E�B���h�E���j�������Ƃ��ɑ��M����郁�b�Z�[�W
		PostQuitMessage(0);// ���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W���|�X�g���āA�A�v���P�[�V�������I������悤�Ɏw��
		break;// switch���̏I��
	case WM_ACTIVATE:		// �E�B���h�E���A�N�e�B�u����A�N�e�B�u�ɂȂ����Ƃ�
	case WM_INPUT:			// DirectInput��RawInput���g�p���Ă���
	case WM_MOUSEMOVE:		// �}�E�X���ړ������Ƃ�
	case WM_LBUTTONDOWN:	// ���{�^���������ꂽ�Ƃ�
	case WM_LBUTTONUP:		// ���{�^���������ꂽ�Ƃ�
	case WM_RBUTTONDOWN:	// �E�{�^���������ꂽ�Ƃ�
	case WM_RBUTTONUP:		// �E�{�^���������ꂽ�Ƃ�
	case WM_MBUTTONDOWN:	// ���{�^���������ꂽ�Ƃ�
	case WM_MBUTTONUP:		// ���{�^���������ꂽ�Ƃ�
	case WM_MOUSEWHEEL:		// �}�E�X�z�C�[������]�����Ƃ�
	case WM_XBUTTONDOWN:	// �T�C�h�{�^���������ꂽ�Ƃ�
	case WM_XBUTTONUP:		// �T�C�h�{�^���������ꂽ�Ƃ�
	case WM_MOUSEHOVER:		// �}�E�X���z�o�[��ԂɂȂ����Ƃ�
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);// �}�E�X�̃��b�Z�[�W������
		break;// switch���̏I��
	case WM_KEYDOWN:		// �L�[�������ꂽ�Ƃ�
	case WM_KEYUP:			// �L�[�������ꂽ�Ƃ�
	case WM_SYSKEYUP:		// �V�X�e���L�[�������ꂽ�Ƃ�
		Keyboard::ProcessMessage(message, wParam, lParam);// �L�[�{�[�h�̃��b�Z�[�W������
		break;// switch���̏I��
	case WM_SYSKEYDOWN:		// �V�X�e���L�[�������ꂽ�Ƃ�
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Alt+Enter�L�[�������ꂽ�ꍇ�A�t���X�N���[�����[�h��؂�ւ���
			if (s_fullscreen)// �t���X�N���[�����[�h�̏ꍇ
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_MY_WINDOW);// �E�B���h�E�X�^�C����ʏ�̃E�B���h�E�X�^�C���ɐݒ�
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);			// �g���X�^�C����ʏ�̃E�B���h�E�X�^�C���ɐݒ�
				int width = 1920; // ����1920�ɐݒ�
				int height = 1080; // ������1080�ɐݒ�
				if (game)game->GetDefaultSize(width, height);// �Q�[���I�u�W�F�N�g�����݂���ꍇ�A�f�t�H���g�̃E�B���h�E�T�C�Y���擾
				ShowWindow(hWnd, SW_SHOWNORMAL);// �E�B���h�E��ʏ�̏�Ԃŕ\��
				// �E�B���h�E�����̃T�C�Y�ɖ߂��Ƃ��ɃT�C�Y��␳����
				RECT rc{};// �E�B���h�E�̃N���C�A���g�̈�̋�`��������
				AdjustWindowRect(&rc, WS_MY_WINDOW, FALSE);// �E�B���h�E�̃N���C�A���g�̈�̋�`�𒲐�
				SetWindowPos(// �E�B���h�E�̈ʒu�ƃT�C�Y��ݒ�
					hWnd, HWND_TOP,											// �E�B���h�E���őO�ʂɕ\��
					0, 0,													// �E�B���h�E�̈ʒu��(0, 0)�ɐݒ�
					width + rc.right - rc.left, height + rc.bottom - rc.top,// �E�B���h�E�̃T�C�Y�𒲐�
					SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED			// �E�B���h�E�̈ʒu��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
				);
			}
			else// �E�B���h�E���[�h�̏ꍇ
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);// �E�B���h�E�X�^�C�����|�b�v�A�b�v�X�^�C���ɐݒ�
				SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST); // �g���X�^�C�����őO�ʂɐݒ�
				SetWindowPos(// �E�B���h�E�̈ʒu�ƃT�C�Y��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
					hWnd,															// �E�B���h�E�n���h��
					HWND_TOP,														// �E�B���h�E���őO�ʂɕ\��
					0, 0, 0, 0,														// �E�B���h�E�̈ʒu��(0, 0)�ɐݒ�
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);		// �E�B���h�E�̈ʒu��ύX�����A�T�C�Y��ύX�����AZ�I�[�_�[��ύX�����A�t���[�����ĕ`�悷��
				ShowWindow(hWnd, SW_SHOWMAXIMIZED); // �E�B���h�E���ő剻���ĕ\��
			}
			s_fullscreen = !s_fullscreen; // �t���X�N���[����Ԃ�؂�ւ���
		}
		break;

	case WM_MENUCHAR:// ���j���[�̃L�����N�^�[���͂����������Ƃ�
		return MAKELRESULT(0, MNC_CLOSE);// ���j���[���A�N�e�B�u�ŁA���[�U�[���j�[���j�b�N��A�N�Z�����[�^�L�[�ɑΉ����Ȃ��L�[���������ꍇ�͖���
	}
	return DefWindowProc(hWnd, message, wParam, lParam);// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ăяo���āA���b�Z�[�W����������
}

/*
*	@brief �Q�[�����I������֐�
*	@details ���̊֐��̓Q�[�����I�����邽�߂Ɏg�p�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ExitGame() noexcept
{
	PostQuitMessage(0);// ���b�Z�[�W�L���[��WM_QUIT���b�Z�[�W���|�X�g���āA�A�v���P�[�V�������I������悤�Ɏw��
}
