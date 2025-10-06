//================================================================================================================
//
// DirectX�̃u���b�N�ݒu�p�̃w�b�_�[�t�@�C�� [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "player.h"

//**********************************************************************************
//*** �}�N����` ***
//**********************************************************************************

//**********************************************************************************
//*** �u���b�N�\���� ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p���Ă��邩
}BLOCK;

//**********************************************************************************
//*** �v���g�^�C�v�錾 ***
//**********************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth, OR_GRAVITY gravity);
void CheckBlockMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fHeight, float fWidth);

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
BLOCK *GetBlock(void);
int GetCollison(void);

#endif // !_BLOCK_H_
