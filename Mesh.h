//=======================================================================================
//
//	  ���b�V�� �N���X��`
//
//=======================================================================================
#pragma once
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_

class CMesh
{
private:
	LPD3DXMESH			m_pD3DMesh;			// D3DXMesh �I�u�W�F�N�g
	DWORD				m_dwNumMaterial;	// �}�e���A����
	D3DMATERIAL9*		m_pMaterial;		// �}�e���A��
	LPDIRECT3DTEXTURE9*	m_ppTexture;		// �e�N�X�`��

public:
	CMesh();								// �R���X�g���N�^
	virtual ~CMesh();						// �f�X�g���N�^
	bool Initialize(LPCTSTR pszFName);		// ���b�V��������
	void Finalize();						// ���b�V�����
	void Draw(D3DXMATRIX& world);			// ���b�V���`��
};

//=======================================================================================
//	End of File
//=======================================================================================