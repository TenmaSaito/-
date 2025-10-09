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
#include "editer.h"

//**********************************************************************************
//*** �}�N����` ***
//**********************************************************************************
#define MAX_BLOCK			(128)	// �u���b�N�̍ő吔
#define EDITBLOCK_NUMBER	(0)		// �G�f�B�^�u���b�N�̔z��ԍ�

//**********************************************************************************
//*** �u���b�N�\���� ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu
	D3DXCOLOR col;			// �F
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

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
BLOCK *GetBlock(void);
int GetCollison(void);
int GetSelectNumber(void);
int GetBlockMax(void);								// �G�f�B�^�ɂĐݒu�����u���b�N�̐�
void SetBlockFromFile(BLOCKFROMEDIT *pbfeBlock);	// �t�@�C������ǂݍ��񂾃u���b�N�̐ݒu

#endif // !_BLOCK_H_
