//=======================================================================================
//
//	  [ DXGrph9_simple ] : DirectX Graphics9 ��ԃV���v���ȃv���O����
//
//=======================================================================================

//-------- �C���N���[�h�w�b�_
#define STRICT
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <chrono>
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_�[
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_�[
#include <mmsystem.h>		// ���ԊǗ��Ŏg�p
#include <stdlib.h>			// �����Ǘ��Ŏg�p

#define _GLOBAL				// common.h �̒��ňӖ�������

#include "define.h"
#include "common.h"
#include "proto.h"

#pragma comment(lib, "winmm.lib")

//---------------------------------------------------------------------------------------
//	���C��
//---------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	MSG		msg;
	DWORD	dwExecLastTime;		// FPS�v���p
	DWORD	dwFPSLastTime;
	DWORD	dwCurrentTime;
	DWORD	dwFrameCount;

	srand(GetTickCount());

	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc,
		0, 0, hInstance, LoadIcon(hInstance, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, CLASS_NAME, NULL};
	if (RegisterClassEx(&wc) == 0) return false;	// �E�B���h�E�N���X��o�^

	// �E�B���h�E���쐬
	g_hWnd = CreateWindow(
		CLASS_NAME, CAPTION_NAME, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH  + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL, NULL, hInstance, NULL);
	if (g_hWnd == NULL) return false;
	ShowWindow(g_hWnd, iCmdShow);		// �E�B���h�E��\��
	UpdateWindow(g_hWnd);				// �E�B���h�E���A�b�v�f�[�g

	// �O���t�B�b�N���̏�����
	bool	bWindow = false;
	if (IDYES == MessageBox(g_hWnd, _T("�E�B���h�E���[�h�Ŏ��s���܂����H"), _T("��ʃ��[�h"), MB_YESNO))
		bWindow = true;
	if (FAILED(InitializeGraphics(g_hWnd, bWindow))) return false;	// �����ŏ�����

	// �e�평����
	timeBeginPeriod(1);									// �^�C�}�[�̕���\���ŏ��ɃZ�b�g
	dwFPSLastTime = dwExecLastTime = timeGetTime();		// ���݂̃V�X�e���^�C�}�[���擾
	dwExecLastTime -= FRAME_RATE;
	dwFrameCount = 0;
	g_FPS = 0;
	g_bWire = false;

	// ���b�Z�[�W���[�v
	msg.message = WM_NULL;
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {		// ���b�Z�[�W���`�F�b�N
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// ���̕ӂŃt���[�����J�E���g
			dwCurrentTime = timeGetTime();					// ���݂̃^�C�}�l���擾
			if (dwCurrentTime - dwFPSLastTime >= 500) {		// 0.5 �b���ƂɌv��
				// �t���[�������v�Z
				g_FPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// �^�C�}�l���X�V
				dwFrameCount = 0;				// �t���[���J�E���^�����Z�b�g
			}

			// ���̕ӂŎ��ԊǗ�
			while (dwCurrentTime - dwExecLastTime >= FRAME_RATE) {	// ��莞�Ԃ��o�߂�����c
				dwExecLastTime += FRAME_RATE;						// �^�C�}�l���X�V

				GameMain();											// �Q�[�����C������
			}
			RenderGraphics();									// �����_�����O���������s
			dwFrameCount++;										// �t���[���J�E���g�� +1
			Sleep(1);				// �������� Windows �ɐ����߂�
		}
	}

	// �O���t�B�b�N���̃N���[���A�b�v
	CleanupGraphics();
	timeEndPeriod(1);				// �^�C�}�[�̕���\�����ɖ߂�

	return msg.wParam;
}

//---------------------------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//---------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:				//----- �E�B���h�E�j���w��������
		PostQuitMessage(0);				// �V�X�e���ɃX���b�h�̏I����v��
		return 0;
	case WM_KEYDOWN:				//----- �L�[�{�[�h�������ꂽ
		switch (wParam) {
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			return 0;
		case 'W':						// [W]�L�[�������ꂽ
			g_bWire = !g_bWire;			// ���C���[�t���[�� ���[�h�𔽓]
			return 0;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================================================================
//	End of File
//=======================================================================================
