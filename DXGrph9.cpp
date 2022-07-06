//=======================================================================================
//
//	  DirectX Graphics9 ���W���[��
//
//=======================================================================================
//-------- �C���N���[�h�w�b�_
#define STRICT
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <stdio.h>			// �����񏈗��ŕK�v
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_

#include "define.h"
#include "common.h"
#include "proto.h"

#include "Mesh.h"

//-------- ���C�u�����̃����N�i�����L�q���Ă����Εʓr�����N�ݒ肪�s�v�ƂȂ�j
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

CMesh	g_mesh;

//---------------------------------------------------------------------------------------
// �O���t�B�b�N���̏�����
//---------------------------------------------------------------------------------------
HRESULT InitializeGraphics(HWND hWnd, bool bWindow)
{
	// �f�o�C�X���e�[�u��
	struct TCreateDevice {
		D3DDEVTYPE type;			// �f�o�C�X�̎��
		DWORD      behavior;		// �f�o�C�X�̓���
	};
	const int c_nMaxDevice = 3;
	const TCreateDevice device[c_nMaxDevice] = {
	    { D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// �n�[�h�E�F�A�V�F�[�_�[���g��
	    { D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
	    { D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
	};
	int		nDev;			// �f�o�C�X��ʃJ�E���^

	// Direct3D�I�u�W�F�N�g�𐶐�
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		MessageBox(hWnd, _T("Direct3D�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// Direct3D �������p�����[�^�̐ݒ�
	ZeroMemory(&g_D3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));
	g_D3DPresentParam.BackBufferWidth        = SCREEN_WIDTH;			// �Q�[����ʃT�C�Y
	g_D3DPresentParam.BackBufferHeight       = SCREEN_HEIGHT;
	g_D3DPresentParam.Windowed               = bWindow;					// �E�B���h�E���[�h��?
	g_D3DPresentParam.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	g_D3DPresentParam.EnableAutoDepthStencil = true;					// �y�o�b�t�@�L��
	g_D3DPresentParam.AutoDepthStencilFormat = D3DFMT_D24S8;			// �y�o�b�t�@�t�H�[�}�b�g
	if (bWindow) {
		// �E�B���h�E���[�h
		g_D3DPresentParam.BackBufferFormat           = D3DFMT_UNKNOWN;	// �o�b�N�o�b�t�@
		g_D3DPresentParam.FullScreen_RefreshRateInHz = 0;				// ���t���b�V�����[�g
		g_D3DPresentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	} else {
		// �t���X�N���[�����[�h
		g_D3DPresentParam.BackBufferFormat           = D3DFMT_X8R8G8B8;			// �o�b�N�o�b�t�@
		g_D3DPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
		g_D3DPresentParam.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
		if(SUCCEEDED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, device[nDev].type, hWnd,	// �f�o�C�X���쐬
					device[nDev].behavior, &g_D3DPresentParam, &g_pD3DDevice))) break;
	}
	if (nDev >= c_nMaxDevice) {
		MessageBox(hWnd, _T("�f�o�C�X�̍쐬�Ɏ��s���܂���"), _T("error"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);

	//-------- ��������R�c�֘A

	// ���C�e�B���O�ݒ�
	D3DLIGHT9 light	= {D3DLIGHT_DIRECTIONAL};
	light.Diffuse	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Ambient	= D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Direction	= D3DXVECTOR3(-5.0f, -1.0f, 1.0f);
	g_pD3DDevice->SetLight(0, &light);
	g_pD3DDevice->LightEnable(0, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);

	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMATRIX	matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(30), ASPECT_RATIO, NEAR_CLIP, FAR_CLIP);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	// ���u�����h�ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`�� �X�e�[�W �X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); // ����1�Ɠ���2���|�����킹��B
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); // ����1�F�e�N�X�`��
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); // ����2�F�f�B�t���[�Y(��{�F)
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//-------- �Q�[���p�I�u�W�F�N�g�̏�����
	InitWave();								// �g�̏�����
	g_mesh.Initialize(_T("asset/land2.x"));

	return S_OK;
}

//---------------------------------------------------------------------------------------
// �O���t�B�b�N���̃N���[���A�b�v
//---------------------------------------------------------------------------------------
HRESULT CleanupGraphics()
{
	//-------- �Q�[���p�I�u�W�F�N�g�̌�n��
	g_mesh.Finalize();
	SAFE_RELEASE(g_pD3DXFont);			// D3DXFont �I�u�W�F�N�g�����
	SAFE_RELEASE(g_pD3DDevice);			// Direct3DDevice �I�u�W�F�N�g�����
	SAFE_RELEASE(g_pD3D);				// Direct3D �I�u�W�F�N�g�����
	return S_OK;
}

//---------------------------------------------------------------------------------------
// �O���t�B�b�N���̃����_�����O
//---------------------------------------------------------------------------------------
HRESULT RenderGraphics()
{
	// �r���[�|�[�g���N���A
//	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(30, 65, 90), 1.0f, 0);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(128, 192, 255), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {	// �V�[���J�n

		if (g_bWire)
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���C���[�t���[��

		DrawMain();									// �`�揈��

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// ��

		g_pD3DDevice->EndScene();					// �V�[���I��
	}

	// �o�b�N�o�b�t�@�ɕ`�悵�����e��\��
	if (FAILED(g_pD3DDevice->Present(NULL, NULL, NULL, NULL))) {
		g_pD3DDevice->Reset(&g_D3DPresentParam);
	}
	return S_OK;
}

//=======================================================================================
//	End of File
//=======================================================================================