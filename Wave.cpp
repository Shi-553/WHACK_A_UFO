
//=======================================================================================
//
//	  波関連モジュール
//
//=======================================================================================
//-------- インクルードヘッダ
#define STRICT
#include <windows.h>		// Windowsプログラムにはこれを付ける
#include <tchar.h>			// 汎用テキスト マッピング
#include <stdio.h>			// 文字列処理で必要
#include <d3d9.h>			// DirectX Graphics 関連のヘッダ
#include <d3dx9.h>			// DirectX Graphics 関連のヘッダ
#include <DirectXMath.h>

#include "define.h"
#include "common.h"
#include "proto.h"
using namespace DirectX;

//-------- 頂点フォーマット
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
//  波初期化
//---------------------------------------------------------------------------------------
HRESULT InitWave(){
    int	i, j, k = 0;
    for (j = 0; j < MAX_WAVE; j++){
        for (i = 0; i < MAX_WAVE; i++){
            // 頂点値を設定
            vertex[0][j][i].pos.x = (i - (MAX_WAVE / 2)) * 1.0f;
            vertex[0][j][i].pos.y = 0.0f;
            vertex[0][j][i].pos.z = (((MAX_WAVE - 1) - j) - (MAX_WAVE / 2)) * 1.0f;
            vertex[0][j][i].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertex[2][j][i] = vertex[1][j][i] = vertex[0][j][i];
            // インデックス値を設定
            if (j < MAX_WAVE - 1){
                index[k++] = (j + 1) * MAX_WAVE + i;
                index[k++] = j * MAX_WAVE + i;
            }
        }
        // 縮退三角形を設定
        if (j < MAX_WAVE - 2){
            index[k++] = j * MAX_WAVE + MAX_WAVE - 1;
            index[k++] = (j + 2) * MAX_WAVE;
        }
    }

    // 現在の頂点格納先バッファ番号
    num = 0;


    // 波を発生
    vertex[num][rand() % MAX_WAVE][rand() % MAX_WAVE].pos.y = 2.0f;


    return S_OK;
}

//=======================================================================================
//	ここの内容を作成する
//---------------------------------------------------------------------------------------
// 波更新
//---------------------------------------------------------------------------------------
//=======================================================================================

void ExecWave(){

    // 最初の10秒間、波を生成
    if (g_dwTicks < 10 * 1000 && (g_dwTicks % (5 * 1000)) == 0){
        vertex[num][rand() % MAX_WAVE][rand() % MAX_WAVE].pos.y += 2.0f;
    }

    // リングバッファ番号作成  numが現在の番号
    int prev = (num +2) % 3;       //1つ前
    int next = (num + 1) % 3;		//１つ後

    // 波の速度、時間の刻み、グリッド幅
    constexpr float c = 0.4f;	//波の伝わる速度
    constexpr float dt = 0.4f;	//時間の刻み
    constexpr float h = 1.0f;	//グリッドの幅
    constexpr float cth = c * c * dt * dt / (h * h); //このパラメータを使う部分を先に計算しておく
    constexpr float cth2 = (2.0f - (4.0f * cth)); //このパラメータを使う部分を先に計算しておく

    // 次のフレームの波の高さを作成
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

    // 次のフレームの頂点ごとの法線の更新 周辺の頂点とのベクトルを作って外積を計算して正規化
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

    //作成した法線を平均化する　周辺の法線を加算して正規化
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

    // 現在の頂点格納先バッファ番号を更新　現在のnextが次のnumになる
    num = next;
}

//---------------------------------------------------------------------------------------
// 波描画
//---------------------------------------------------------------------------------------
void DrawWave(){
    // ワールドマトリックス設定
    D3DXMATRIX	matWorld;
    D3DXMatrixIdentity(&matWorld);
    g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

    // マテリアル設定
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

    // 頂点フォーマットフラグを設定
    g_pD3DDevice->SetFVF(FVF_VERTEX);

    // 描画を実行
    g_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, MAX_WAVE * MAX_WAVE,
                                         (MAX_WAVE + 1) * (MAX_WAVE - 1) * 2 - 4,
                                         index, D3DFMT_INDEX16,
                                         vertex[num], sizeof(VERTEX));
}

//=======================================================================================
//	End of File
//=======================================================================================
