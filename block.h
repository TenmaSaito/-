//================================================================================================================
//
// DirectXのブロック設置用のヘッダーファイル [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "player.h"
#include "editer.h"

//**********************************************************************************
//*** マクロ定義 ***
//**********************************************************************************
#define MAX_BLOCK			(128)	// ブロックの最大数
#define EDITBLOCK_NUMBER	(0)		// エディタブロックの配列番号

//**********************************************************************************
//*** ブロック構造体 ***
//**********************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posOld;		// 過去の位置
	D3DXCOLOR col;			// 色
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用しているか
}BLOCK;

//**********************************************************************************
//*** プロトタイプ宣言 ***
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
int GetBlockMax(void);								// エディタにて設置したブロックの数
void SetBlockFromFile(BLOCKFROMEDIT *pbfeBlock);	// ファイルから読み込んだブロックの設置

#endif // !_BLOCK_H_
