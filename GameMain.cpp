//=======================================================================================
//
//	  �Q�[�����C���A�`�惂�W���[��
//
//=======================================================================================
//-------- �C���N���[�h�w�b�_
#define STRICT
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <stdio.h>			// �����񏈗��ŕK�v
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_�[
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_�[

#include "define.h"
#include "common.h"
#include "proto.h"

#include "Mesh.h"

extern CMesh	g_mesh;

//---------------------------------------------------------------------------------------
// �Q�[�����C�������i���C�����[�v����R�[�������j
//---------------------------------------------------------------------------------------
void GameMain()
{
	TCHAR	str[256];

	g_szDebug[0] = _T('\0');		// �f�o�b�O�X�g�����O������

	// �e�o�r����ʂɕ`�悷�邽�߂̕�������쐬
	_stprintf(str, _T("FPS = %d\n"), g_FPS);
	lstrcat(g_szDebug, str);

	//----- �����ɃQ�[���{�̏���
	switch (g_nStatus) {
	case 0:
		g_fAngle = D3DXToRadian(45.0f);
		g_fPos   = 7.0f;
		g_dwTicks = 0;

		g_nStatus = 1;
		// THRU
	case 1:
		g_dwTicks++;
		if (GetAsyncKeyState(VK_LEFT ) & 0x8000) g_fAngle += D3DXToRadian(1);
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) g_fAngle -= D3DXToRadian(1);
		if (GetAsyncKeyState(VK_UP   ) & 0x8000) g_fPos   += 1.0f;
		if (GetAsyncKeyState(VK_DOWN ) & 0x8000) g_fPos   -= 1.0f;

		ExecWave();						// �g�̍X�V
		break;
	}
	_stprintf(str, _T("PosY  = %.0f\nAngle = %d\n"), g_fPos, (int)D3DXToDegree(g_fAngle));
	lstrcat(g_szDebug, str);
}

//---------------------------------------------------------------------------------------
// �`�揈���iRenderGraphics ����R�[�������j
//---------------------------------------------------------------------------------------
void DrawMain()
{
	//----- �����ɕ`�揈��
	// �r���[�}�g���b�N�X�ݒ�
	D3DXMATRIX	matView, matRotate;
	D3DXMatrixRotationY(&matRotate, g_fAngle);
	D3DXVECTOR3 vecView(0.0f, g_fPos, -70.0f);
	D3DXVec3TransformCoord(&vecView, &vecView, &matRotate);
	D3DXMatrixLookAtLH(&matView,
		&vecView,								// ���_���W
		&D3DXVECTOR3( 0.0f,  0.0f,  0.0f),		// �����_���W
		&D3DXVECTOR3( 0.0f,  1.0f,  0.0f));		// �A�b�v�x�N�g��
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix, 0.0f, -2.0f, 0.0f);
	g_mesh.Draw(matrix);

	DrawWave();							// �g�̕`��

	// �f�o�b�O�X�g�����O�`��
	RECT	rcStr = {0, 0, 640, 480};
	g_pD3DXFont->DrawText(NULL, g_szDebug, -1, &rcStr, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
}

//=======================================================================================
//	End of File
//=======================================================================================
