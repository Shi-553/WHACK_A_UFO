//=======================================================================================
//
//	  �O���[�o���ϐ��֘A
//
//=======================================================================================

#ifndef _H_COMMON
#define _H_COMMON

#ifdef _GLOBAL
#define extern
#endif
extern LPDIRECT3D9					g_pD3D;					// Direct3D8 �I�u�W�F�N�g
extern LPDIRECT3DDEVICE9			g_pD3DDevice;			// Direct3DDevice8 �I�u�W�F�N�g
extern D3DPRESENT_PARAMETERS		g_D3DPresentParam;		// PRESENT PARAMETERS
extern LPD3DXFONT					g_pD3DXFont;			// D3DXFont �I�u�W�F�N�g

extern HWND							g_hWnd;					// �E�B���h�E�n���h��
extern int							g_FPS;					// �t���[�����J�E���g�p
extern DWORD						g_dwTicks;				// �^�C���J�E���^

extern TCHAR						g_szDebug[1024];		// �f�o�b�O�p�X�g�����O�f�[�^

extern bool							g_bWire;				// ���C���[�t���[���`��
extern int							g_nStatus;				// �t�F�[�Y

extern float						g_fAngle;				// �J�����p�x
extern float						g_fPos;					// �J��������

#endif

//=======================================================================================
//	End of File
//=======================================================================================
