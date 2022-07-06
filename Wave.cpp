
//=======================================================================================
//
//	  �g�֘A���W���[��
//
//=======================================================================================
//-------- �C���N���[�h�w�b�_
#define STRICT
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <stdio.h>			// �����񏈗��ŕK�v
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <DirectXMath.h>

#include "define.h"
#include "common.h"
#include "proto.h"
using namespace DirectX;

//-------- ���_�t�H�[�}�b�g
typedef struct{
    XMFLOAT3	pos;
    XMFLOAT3	nor;

} VERTEX;

#define FVF_VERTEX	(D3DFVF_XYZ|D3DFVF_NORMAL)//|D3DFVF_DIFFUSE|D3DFVF_SPECULAR)

#define MAX_WAVE	100
#define MAX_INDEX	((MAX_WAVE+1)*(MAX_WAVE-1)*2-2)

VERTEX	vertex[3][MAX_WAVE][MAX_WAVE];
int		num;
WORD	index[MAX_INDEX];

#define MAX_NORMAL_X	((MAX_WAVE-1)*2)
#define MAX_NORMAL_Y	(MAX_WAVE-1)

XMFLOAT3	normal[MAX_NORMAL_Y][MAX_NORMAL_X];

//---------------------------------------------------------------------------------------
//  �g������
//---------------------------------------------------------------------------------------
HRESULT InitWave(){
    int	i, j, k = 0;
    for (j = 0; j < MAX_WAVE; j++){
        for (i = 0; i < MAX_WAVE; i++){
            // ���_�l��ݒ�
            vertex[0][j][i].pos.x = (i - (MAX_WAVE / 2)) * 1.0f;
            vertex[0][j][i].pos.y = 0.0f;
            vertex[0][j][i].pos.z = (((MAX_WAVE - 1) - j) - (MAX_WAVE / 2)) * 1.0f;
            vertex[0][j][i].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertex[2][j][i] = vertex[1][j][i] = vertex[0][j][i];
            // �C���f�b�N�X�l��ݒ�
            if (j < MAX_WAVE - 1){
                index[k++] = (j + 1) * MAX_WAVE + i;
                index[k++] = j * MAX_WAVE + i;
            }
        }
        // �k�ގO�p�`��ݒ�
        if (j < MAX_WAVE - 2){
            index[k++] = j * MAX_WAVE + MAX_WAVE - 1;
            index[k++] = (j + 2) * MAX_WAVE;
        }
    }

    // ���݂̒��_�i�[��o�b�t�@�ԍ�
    num = 0;


    // �g�𔭐�
    vertex[num][rand() % MAX_WAVE][rand() % MAX_WAVE].pos.y = 2.0f;


    return S_OK;
}

//=======================================================================================
//	�����̓��e���쐬����
//---------------------------------------------------------------------------------------
// �g�X�V
//---------------------------------------------------------------------------------------
//=======================================================================================

void ExecWave(){

    // �ŏ���10�b�ԁA�g�𐶐�
    if (g_dwTicks < 10 * 1000 && (g_dwTicks % (5 * 1000)) == 0){
        vertex[num][rand() % MAX_WAVE][rand() % MAX_WAVE].pos.y += 2.0f;
    }

    // �����O�o�b�t�@�ԍ��쐬  num�����݂̔ԍ�
    int prev = (num +2) % 3;       //1�O
    int next = (num + 1) % 3;		//�P��

    // �g�̑��x�A���Ԃ̍��݁A�O���b�h��
    constexpr float c = 0.4f;	//�g�̓`��鑬�x
    constexpr float dt = 0.4f;	//���Ԃ̍���
    constexpr float h = 1.0f;	//�O���b�h�̕�
    constexpr float cth = c * c * dt * dt / (h * h); //���̃p�����[�^���g���������Ɍv�Z���Ă���
    constexpr float cth2 = (2.0f - (4.0f * cth)); //���̃p�����[�^���g���������Ɍv�Z���Ă���

    // ���̃t���[���̔g�̍������쐬
    for (int j = 1; j < MAX_WAVE - 1; j++){
        for (int i = 1; i < MAX_WAVE - 1; i++){
            float temp0 = cth * (vertex[num][j][i + 1].pos.y +
                                 vertex[num][j][i - 1].pos.y +
                                 vertex[num][j + 1][i].pos.y +
                                 vertex[num][j - 1][i].pos.y);
            float temp1 = cth2 * vertex[num][j][i].pos.y - vertex[prev][j][i].pos.y;

            vertex[next][j][i].pos.y = temp0 + temp1;
        }
    }

    // ���̃t���[���̒��_���Ƃ̖@���̍X�V ���ӂ̒��_�Ƃ̃x�N�g��������ĊO�ς��v�Z���Đ��K��
    for (int j = 0; j < MAX_WAVE - 1; j++){
        for (int i = 0; i < MAX_WAVE - 1; i++){
            auto p0 = XMLoadFloat3(&vertex[next][j + 1][i].pos);
            auto p1 = XMLoadFloat3(&vertex[next][j][i].pos);
            auto p2 = XMLoadFloat3(&vertex[next][j + 1][i + 1].pos);

            auto n0 = XMVector3NormalizeEst( XMVector3Cross((p1 - p0), (p2 - p1)));
            
            XMStoreFloat3(& normal[j][i * 2], n0);

            auto q0 = XMLoadFloat3(&vertex[next][j][i].pos);
            auto q1 = XMLoadFloat3(&vertex[next][j][i + 1].pos);
            auto q2 = XMLoadFloat3(&vertex[next][j + 1][i + 1].pos);

            auto n1 = XMVector3NormalizeEst(XMVector3Cross((q1 - q0), (q2 - q1)));

            XMStoreFloat3(&normal[j][i * 2 + 1], n1);

        }
    }

    //�쐬�����@���𕽋ω�����@���ӂ̖@�������Z���Đ��K��
    for (int j = 0; j < MAX_WAVE - 1; j++){
        for (int i = 0; i < MAX_WAVE-1; i++){
            auto n0 = XMLoadFloat3(&normal[(j + MAX_NORMAL_Y - 1) % MAX_NORMAL_Y][(i * 2 + MAX_NORMAL_X - 2) % MAX_NORMAL_X]);
            auto n1 = XMLoadFloat3(&normal[(j + MAX_NORMAL_Y - 1) % MAX_NORMAL_Y][(i * 2 + MAX_NORMAL_X - 1) % MAX_NORMAL_X]);
            auto n2 = XMLoadFloat3(&normal[(j + MAX_NORMAL_Y - 1) % MAX_NORMAL_Y][i * 2]);
            auto n3 = XMLoadFloat3(&normal[j][(i * 2 + MAX_NORMAL_X - 1) % MAX_NORMAL_X]);
            auto n4 = XMLoadFloat3(&normal[j][i * 2]);
            auto n5 = XMLoadFloat3(&normal[j][(i * 2 + 1) % MAX_NORMAL_X]);

            XMStoreFloat3(&vertex[next][j][i].nor, XMVector3NormalizeEst(n0 + n1 + n2 + n3 + n4 + n5));
        }
    }

    // ���݂̒��_�i�[��o�b�t�@�ԍ����X�V�@���݂�next������num�ɂȂ�
    num = next;
}

//---------------------------------------------------------------------------------------
// �g�`��
//---------------------------------------------------------------------------------------
void DrawWave(){
    // ���[���h�}�g���b�N�X�ݒ�
    D3DXMATRIX	matWorld;
    D3DXMatrixIdentity(&matWorld);
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

    // �}�e���A���ݒ�
    D3DMATERIAL9 material;
    material.Diffuse.r = 0.25f;
    material.Diffuse.g = 0.8f;
    material.Diffuse.b = 1.0f;
    material.Diffuse.a = 0.5f;
    material.Ambient = material.Diffuse;
    material.Specular = material.Diffuse;
    material.Power = 1.0f;
    material.Emissive.r = 0.0f;
    material.Emissive.g = 0.0f;
    material.Emissive.b = 0.0f;
    material.Emissive.a = 0.0f;
    g_pD3DDevice->SetMaterial(&material);
    g_pD3DDevice->SetTexture(0, NULL);

    // ���_�t�H�[�}�b�g�t���O��ݒ�
    g_pD3DDevice->SetFVF(FVF_VERTEX);

    // �`������s
    g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, MAX_WAVE * MAX_WAVE,
                                         (MAX_WAVE + 1) * (MAX_WAVE - 1) * 2 - 4,
                                         index, D3DFMT_INDEX16,
                                         vertex[num], sizeof(VERTEX));
}

//=======================================================================================
//	End of File
//=======================================================================================
