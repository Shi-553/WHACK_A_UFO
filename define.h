//=======================================================================================
//
//	  �萔��`�A�}�N����`
//
//=======================================================================================

#ifndef _H_DEFINE
#define _H_DEFINE

//-------- �}�N��
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){ delete[] x; x=NULL; }
#endif

//-------- �萔��`
#define CLASS_NAME		_T("NewPaddle")		// �E�B���h�E�N���X��
#define CAPTION_NAME	_T("W�ŕ\���ؑց@�J�[�\���L�[�ŉ�]")	// �L���v�V������

#define SCREEN_WIDTH	1024		// �X�N���[���̕�
#define SCREEN_HEIGHT	768			// �X�N���[���̍���
#define FRAME_RATE		(1000/60)	// �t���[�����[�g

#define ASPECT_RATIO	((float)SCREEN_WIDTH/SCREEN_HEIGHT)	// �A�X�y�N�g��
#define NEAR_CLIP		1.0f								// �j�A�N���b�v���s������
#define FAR_CLIP		30000.0f							// �t�@�[�N���b�v���s������

#endif

//=======================================================================================
//	End of File
//=======================================================================================