//=======================================================================================
//
//	  ���b�V�� �N���X
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_
#include "define.h"
#include "common.h"

#include "Mesh.h"

//---------------------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------------------
CMesh::CMesh()
{
	m_pD3DMesh = NULL;
	m_dwNumMaterial = 0;
	m_pMaterial = NULL;
	m_ppTexture = NULL;
}

//---------------------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------------------
CMesh::~CMesh()
{
	Finalize();
}

//---------------------------------------------------------------------------------------
// ���b�V��������
//---------------------------------------------------------------------------------------
bool CMesh::Initialize(LPCTSTR pszFName)
{
	TCHAR			szMsg[MAX_PATH + 32];
	TCHAR			szDir[_MAX_DIR];
	TCHAR			szCurrentDir[_MAX_PATH];

	LPD3DXBUFFER	pD3DXMtrlBuffer = NULL;

	// �t�H���_���𒊏o
	_tsplitpath(pszFName, NULL, szDir, NULL, NULL);

	// �w�t�@�C�����烁�b�V���f�[�^��ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pszFName, D3DXMESH_SYSTEMMEM, g_pD3DDevice,
		NULL, &pD3DXMtrlBuffer, NULL, &m_dwNumMaterial, &m_pD3DMesh))) {
		_stprintf(szMsg, _T("X�t�@�C��(%s)�̓ǂݍ��݂Ɏ��s���܂����B"), pszFName);
		MessageBox(NULL, szMsg, NULL, MB_OK);
		return false;
	}

	// �@���������ꍇ�͋����I�ɒǉ�
	if ((m_pD3DMesh->GetFVF() & D3DFVF_NORMAL) == 0) {
		LPD3DXMESH pMeshTmp = m_pD3DMesh;
		pMeshTmp->CloneMeshFVF(pMeshTmp->GetOptions(), pMeshTmp->GetFVF() | D3DFVF_NORMAL,
			g_pD3DDevice, &m_pD3DMesh);
		SAFE_RELEASE(pMeshTmp);
		D3DXComputeNormals(m_pD3DMesh, NULL);
	}

	// �J�����g�f�B���N�g����ύX
	if (szDir[0]) {
		GetCurrentDirectory(_MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szDir);
	}

	// �}�e���A���ǂݍ���
	D3DXMATERIAL* pD3DMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new D3DMATERIAL9[m_dwNumMaterial];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwNumMaterial];
	for (DWORD i = 0; i < m_dwNumMaterial; i++) { 
		m_pMaterial[i] = pD3DMaterials[i].MatD3D;
		m_pMaterial[i].Ambient = m_pMaterial[i].Diffuse;
		m_ppTexture[i] = NULL;
		if (pD3DMaterials[i].pTextureFilename && pD3DMaterials[i].pTextureFilename[0]) {
			// �e�N�X�`���t�@�C����ǂݍ���
			if (FAILED(D3DXCreateTextureFromFileA(g_pD3DDevice,
				pD3DMaterials[i].pTextureFilename, &m_ppTexture[i]))) {
				_stprintf(szMsg, _T("�e�N�X�`��(%hs)�̓ǂݍ��݂Ɏ��s���܂����B"),
					pD3DMaterials[i].pTextureFilename);
				MessageBox(NULL, szMsg, NULL, MB_OK);
			}
		}
	}

	// �J�����g�f�B���N�g�������ɖ߂�
	if (szDir[0])
		SetCurrentDirectory(szCurrentDir);

	pD3DXMtrlBuffer->Release();

	return true;
}

//---------------------------------------------------------------------------------------
// ���b�V�����
//---------------------------------------------------------------------------------------
void CMesh::Finalize()
{
	// �e�N�X�`�� �I�u�W�F�N�g�����
	if (m_ppTexture) {
		for (DWORD i = 0; i < m_dwNumMaterial; i++) {
			SAFE_RELEASE(m_ppTexture[i]);
		}
		SAFE_DELETE_ARRAY(m_ppTexture);
	}
	SAFE_DELETE_ARRAY(m_pMaterial);

	SAFE_RELEASE(m_pD3DMesh);	// ���b�V�� �I�u�W�F�N�g�����
}

//---------------------------------------------------------------------------------------
// ���b�V���`��
//---------------------------------------------------------------------------------------
void CMesh::Draw(D3DXMATRIX& world)
{
	// ���[���h �}�g���b�N�X�ݒ�
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwNumMaterial; i++) {
		g_pD3DDevice->SetMaterial(&m_pMaterial[i]);
		g_pD3DDevice->SetTexture(0, m_ppTexture[i]);	// �e�N�X�`����ݒ�
		m_pD3DMesh->DrawSubset(i);						// �`������s
	}
}

//=======================================================================================
//	End of File
//=======================================================================================