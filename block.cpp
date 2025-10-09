//================================================================================================================
//
// DirectXのブロックの設置用のcppファイル [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "block.h"
#include "input.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define EDITBLOCK_UP				DIK_I					// エディタブロックを上にずらす
#define EDITBLOCK_DOWN				DIK_K					// エディタブロックを下にずらす
#define EDITBLOCK_LEFT				DIK_J					// エディタブロックを左にずらす
#define EDITBLOCK_RIGHT				DIK_L					// エディタブロックを右にずらす
#define EDITBLOCK_WIDTH_UP			DIK_M					// エディタブロックを右に伸ばす
#define EDITBLOCK_WIDTH_DOWN		DIK_N					// エディタブロックを左に縮める
#define EDITBLOCK_HEIGHT_UP			DIK_O					// エディタブロックを下に伸ばす
#define EDITBLOCK_HEIGHT_DOWN		DIK_U					// エディタブロックを上に縮める
#define SELECTBLOCK_UP				DIK_T					// 選択しているブロックを上にずらす
#define SELECTBLOCK_DOWN			DIK_G					// 選択しているブロックを下にずらす
#define SELECTBLOCK_LEFT			DIK_F					// 選択しているブロックを左にずらす
#define SELECTBLOCK_RIGHT			DIK_H					// 選択しているブロックを右にずらす
#define SELECTBLOCK_NEXT			DIK_UP					// 選択番号を一つ次にずらす
#define SELECTBLOCK_LAST			DIK_DOWN				// 選択番号を一つ前に戻す
#define EDITBLOCK_CREATE			DIK_RETURN				// エディタブロックの位置にエディタブロックの状態のブロックを配置する
#define SELECTBLOCK_DELETE			DIK_BACK				// 選択しているブロックを消す

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// 頂点バッファのポインタ
BLOCK g_aBlock[MAX_BLOCK] = {};					// ブロックの情報
BLOCK g_aBlockDoppel[MAX_BLOCK] = {};			// ブロックの情報のコピー(データの管理に使用 ※基本的に使用しないこと！)
int g_nCheckCollision;
int g_nSelectBlock;								//選択中のブロック
int g_nCounterBlock;							//生成されたブロックの数
bool g_bChangedBlock;							// ブロックの情報が変化したか
int g_nblinkCounter;							//点滅カウンタ
bool g_bBlink;									//点滅切り替え

//================================================================================================================
// --- ブロックの初期化 ---
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスのポインタ,デバイスの取得
	VERTEX_2D* pVtx = NULL;							// 頂点情報へのポインタ
	int nCntBlock = 0;
	g_nCheckCollision = 0;
	g_nSelectBlock = 0;
	g_bChangedBlock = false;
	g_nCounterBlock = 0;
	g_nblinkCounter = 0;
	g_bBlink = true;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = g_aBlockDoppel[nCntBlock].pos = D3DXVECTOR3_NULL;
		g_aBlock[nCntBlock].posOld = g_aBlockDoppel[nCntBlock].posOld = D3DXVECTOR3_NULL;
		g_aBlock[nCntBlock].col = g_aBlockDoppel[nCntBlock].col = D3DXCOLOR_NULL;
		g_aBlock[nCntBlock].fHeight = g_aBlockDoppel[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].fWidth = g_aBlockDoppel[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].bUse = g_aBlockDoppel[nCntBlock].bUse = false;
	}

	//SetBlock(D3DXVECTOR3(200.0f, 670.0f, 0.0f), 50.0f, 50.0f);
	g_aBlock[EDITBLOCK_NUMBER].pos = D3DXVECTOR3(200.0f, 670.0f, 0.0f);
	g_aBlock[EDITBLOCK_NUMBER].posOld = D3DXVECTOR3(200.0f, 670.0f, 0.0f);
	g_aBlock[EDITBLOCK_NUMBER].fHeight = 50.0f;
	g_aBlock[EDITBLOCK_NUMBER].fWidth = 50.0f;
	g_aBlock[EDITBLOCK_NUMBER].bUse = true;
	g_nCounterBlock++;
	g_aBlockDoppel[EDITBLOCK_NUMBER].pos = D3DXVECTOR3(200.0f, 670.0f, 0.0f);
	g_aBlockDoppel[EDITBLOCK_NUMBER].posOld = D3DXVECTOR3(200.0f, 670.0f, 0.0f);
	g_aBlockDoppel[EDITBLOCK_NUMBER].fHeight = 50.0f;
	g_aBlockDoppel[EDITBLOCK_NUMBER].fWidth = 50.0f;
	g_aBlockDoppel[EDITBLOCK_NUMBER].bUse = true;

/*
	SetBlock(D3DXVECTOR3(200.0f, 620.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 570.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 520.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 470.0f, 0.0f), 50.0f, 50.0f);
*/
	/*SetBlock(D3DXVECTOR3(300.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(350.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(400.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(450.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(500.0f, 710.0f, 0.0f), 50.0f, 50.0f);*/

	/*** テクスチャの読み込み ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block.png",
		&g_pTextureBlock);

	/*** 頂点バッファの生成 ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** 頂点座標の設定の設定 ***/
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
		pVtx[3].pos.z = 0.0f;

		/*** rhwの設定 ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** 頂点カラー設定 ***/
		pVtx[0].col = g_aBlock[nCntBlock].col;
		pVtx[1].col = g_aBlock[nCntBlock].col;
		pVtx[2].col = g_aBlock[nCntBlock].col;
		pVtx[3].col = g_aBlock[nCntBlock].col;

		/*** テクスチャ座標の設定 ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- ブロックの終了処理 ---
//================================================================================================================
void UninitBlock(void)
{
	/*** テクスチャの破棄 ***/
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	/*** 頂点バッファの破棄 ***/
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// --- ブロックの更新処理 ---
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;

	bool bUseDebugPlay = GetDebugPlay();		// デバッグプレイが有効か取得

	if (bUseDebugPlay == true)
	{// デバッグプレイ中
		g_aBlock[EDITBLOCK_NUMBER].bUse = false;	// 選択ブロックの無効化
	}
	else
	{
		g_aBlock[EDITBLOCK_NUMBER].bUse = true;
	}

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		}
	}

	/*** 頂点バッファの設定 ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	if (bUseDebugPlay != true)
	{// デバッグプレイ中でないなら
		if (GetKeyboardRepeat(EDITBLOCK_HEIGHT_UP))
		{
			g_aBlock[EDITBLOCK_NUMBER].fHeight += 1.0f;
		}
		else if (GetKeyboardRepeat(EDITBLOCK_HEIGHT_DOWN))
		{
			g_aBlock[EDITBLOCK_NUMBER].fHeight -= 1.0f;
		}

		if (GetKeyboardRepeat(EDITBLOCK_WIDTH_UP))
		{
			g_aBlock[EDITBLOCK_NUMBER].fWidth += 1.0f;
		}
		else if (GetKeyboardRepeat(EDITBLOCK_WIDTH_DOWN))
		{
			g_aBlock[EDITBLOCK_NUMBER].fWidth -= 1.0f;
		}

		if (GetKeyboardRepeat(EDITBLOCK_UP))
		{
			g_aBlock[EDITBLOCK_NUMBER].pos.y -= 1.0f;
		}
		else if (GetKeyboardRepeat(EDITBLOCK_DOWN))
		{
			g_aBlock[EDITBLOCK_NUMBER].pos.y += 1.0f;
		}

		if (GetKeyboardRepeat(EDITBLOCK_LEFT))
		{
			g_aBlock[EDITBLOCK_NUMBER].pos.x -= 1.0f;
		}
		else if (GetKeyboardRepeat(EDITBLOCK_RIGHT))
		{
			g_aBlock[EDITBLOCK_NUMBER].pos.x += 1.0f;
		}

		if (GetKeyboardRepeat(EDITBLOCK_CREATE))
		{
			//g_aBlock[g_nSelectBlock].col = D3DXCOLOR_NULL;
			SetBlock(g_aBlock[EDITBLOCK_NUMBER].pos, g_aBlock[EDITBLOCK_NUMBER].fWidth, g_aBlock[EDITBLOCK_NUMBER].fHeight);
			g_nSelectBlock = g_nCounterBlock - 1;

		}

		if (GetKeyboardRepeat(SELECTBLOCK_NEXT) && g_nCounterBlock > 1)
		{
			if ((g_nCounterBlock - 1) > g_nSelectBlock)
			{
				g_nSelectBlock++;
			}
			else if ((g_nCounterBlock - 1) == g_nSelectBlock)
			{
				g_nSelectBlock = EDITBLOCK_NUMBER + 1;
			}
		}
		else if (GetKeyboardRepeat(SELECTBLOCK_LAST) && g_nCounterBlock > 1)
		{
			g_nSelectBlock--;

			if (g_nSelectBlock > (EDITBLOCK_NUMBER))
			{

			}
			else if (g_nSelectBlock == EDITBLOCK_NUMBER)
			{
				g_nSelectBlock = (g_nCounterBlock - 1);
			}
		}
		else if (GetKeyboardRepeat(SELECTBLOCK_DELETE) && g_nCounterBlock > 1)
		{
			if (g_nSelectBlock < g_nCounterBlock)
			{
				g_aBlock[g_nSelectBlock].pos = D3DXVECTOR3_NULL;
				g_aBlock[g_nSelectBlock].posOld = D3DXVECTOR3_NULL;
				g_aBlock[g_nSelectBlock].fWidth = 0.0f;
				g_aBlock[g_nSelectBlock].fHeight = 0.0f;
				g_aBlock[g_nSelectBlock].col = D3DXCOLOR_NULL;
				g_aBlock[g_nSelectBlock].bUse = false;
				g_nCounterBlock--;

				if (g_nCounterBlock < 2)
				{
					g_nSelectBlock = 0;
				}
				else
				{
					for (int nCnt = g_nSelectBlock; nCnt < g_nCounterBlock; nCnt++)
					{
						//一つ後ろのブロック情報を格納
						g_aBlock[nCnt].pos = g_aBlock[nCnt + 1].pos;
						g_aBlock[nCnt].posOld = g_aBlock[nCnt + 1].posOld;
						g_aBlock[nCnt].fWidth = g_aBlock[nCnt + 1].fWidth;
						g_aBlock[nCnt].fHeight = g_aBlock[nCnt + 1].fHeight;
						g_aBlock[nCnt].col = g_aBlock[nCnt + 1].col;
						g_aBlock[nCnt].bUse = true;
						g_aBlock[nCnt + 1].bUse = false;
					}

					g_nSelectBlock = g_nCounterBlock - 1;
				}
			}
		}

		if (g_nCounterBlock > 1)
		{
			if (GetKeyboardRepeat(SELECTBLOCK_UP))
			{
				g_aBlock[g_nSelectBlock].pos.y -= 1.0f;
			}
			else if (GetKeyboardRepeat(SELECTBLOCK_DOWN))
			{
				g_aBlock[g_nSelectBlock].pos.y += 1.0f;
			}

			if (GetKeyboardRepeat(SELECTBLOCK_LEFT))
			{
				g_aBlock[g_nSelectBlock].pos.x -= 1.0f;
			}
			else if (GetKeyboardRepeat(SELECTBLOCK_RIGHT))
			{
				g_aBlock[g_nSelectBlock].pos.x += 1.0f;
			}
		}
	}

	//生成前ブロック点滅
	/*g_nblinkCounter++;
	if ((g_nblinkCounter % 30) == 0)
	{
		g_bBlink = g_bBlink ^ true;
		if (g_bBlink == true)
		{
			g_aBlock[g_nCounterBlock].col.a = 0.0f;
		}
		else
		{
			g_aBlock[g_nCounterBlock].col.a = 1.0f;
		}
	}*/

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// コピーデータと見比べて、変化がないか確認
		if (g_bChangedBlock == false && nCntBlock != 0)
		{
			if (g_aBlock[nCntBlock].pos != g_aBlockDoppel[nCntBlock].pos) 
			{ SetEnableChanged(true); g_bChangedBlock = true; }
			if (g_aBlock[nCntBlock].col != g_aBlockDoppel[nCntBlock].col) 
			{ SetEnableChanged(true); g_bChangedBlock = true; }
			if (g_aBlock[nCntBlock].fWidth != g_aBlockDoppel[nCntBlock].fWidth)
			{ SetEnableChanged(true); g_bChangedBlock = true; }
			if (g_aBlock[nCntBlock].fHeight != g_aBlockDoppel[nCntBlock].fHeight)
			{ SetEnableChanged(true); g_bChangedBlock = true; }
			if (g_aBlock[nCntBlock].bUse != g_aBlockDoppel[nCntBlock].bUse) 
			{ SetEnableChanged(true); g_bChangedBlock = true; }
		}

		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** 頂点座標の設定の設定 ***/
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth);
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight);
			pVtx[3].pos.z = 0.0f;

			if (bUseDebugPlay == true)
			{// デバッグプレイ中
				pVtx[0].col = g_aBlock[nCntBlock].col;
				pVtx[1].col = g_aBlock[nCntBlock].col;
				pVtx[2].col = g_aBlock[nCntBlock].col;
				pVtx[3].col = g_aBlock[nCntBlock].col;
			}
			else if (nCntBlock == EDITBLOCK_NUMBER)
			{
				pVtx[0].col = D3DXCOLOR(0, 0, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0, 0, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0, 0, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0, 0, 1.0f, 1.0f);
			}
			else if(nCntBlock == g_nSelectBlock)
			{
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				pVtx[0].col = g_aBlock[nCntBlock].col;
				pVtx[1].col = g_aBlock[nCntBlock].col;
				pVtx[2].col = g_aBlock[nCntBlock].col;
				pVtx[3].col = g_aBlock[nCntBlock].col;
			}
		}

		pVtx += 4;
	}

	/*** 頂点バッファの設定を終了 ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- ブロックの描画処理 ---
//================================================================================================================
void DrawBlock(void)
{
	/*** デバイスの取得 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** 頂点バッファをデータストリームに設定 ***/
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	/*** 頂点フォーマットの設定 ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** テクスチャの設定 ***/
	pDevice->SetTexture(0, g_pTextureBlock);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** ポリゴンの描画 ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				4 * nCntBlock,								// 描画する最初の頂点インデックス
				2);											// 描画するプリミティブの数
		}
	}
}

//================================================================================================================
// --- ブロックの当たり判定処理 ---
//================================================================================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, OR_GRAVITY gravity)
{
	bool bLand = false;					// 空中に浮かんでいる判定で初期化
	bool bHitHead = false;
	bool bMove[4] = { false };
	D3DXVECTOR3 posBefore = *pPos;		// 調整前の位置

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x + fWidth * 0.5f > g_aBlock[nCntBlock].pos.x
				&& pPos->x - fWidth * 0.5f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
				&& pPos->y > g_aBlock[nCntBlock].pos.y
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{
#if 0
				if ((pPos->y > (g_aBlock[nCntBlock].pos.y)
					&& pPos->y - fHeight < (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)))
				{
					if (g_aBlock[nCntBlock].pos.x != g_aBlock[nCntBlock].posOld.x)
					{
						pPos->x += g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].posOld.x;

						if (posBefore.y <= g_aBlock[nCntBlock].pos.y)
						{
							bMove[0] = true;
						}
						else
						{
							bMove[1] = true;
						}
					}
				}
				
				if (pPos->x + fWidth * 0.5f >= (g_aBlock[nCntBlock].pos.x)
					&& pPos->x - fWidth * 0.5f <= (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
				{
					if (g_aBlock[nCntBlock].pos.y != g_aBlock[nCntBlock].posOld.y)
					{
						pPos->y += g_aBlock[nCntBlock].posOld.y - g_aBlock[nCntBlock].pos.y;

						if (posBefore.y <= g_aBlock[nCntBlock].pos.y)
						{
							bMove[2] = true;
						}
						else
						{
							bMove[3] = true;
						}

						bMove[1] = true;
					}
				}
#endif
				if ((pPosOld->y <= g_aBlock[nCntBlock].pos.y
					&& (pPos->y >= g_aBlock[nCntBlock].pos.y)))
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						bLand = true;							// 着地判定に
					}
					else
					{
						bHitHead = true;
					}
				}

				if ((pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
					&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight))
				{
					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						bLand = true;								// 着地判定に
					}
					else
					{
						bHitHead = true;
					}
				}

				if ((pPosOld->y > g_aBlock[nCntBlock].pos.y
					|| pPosOld->y + fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
					|| (pPosOld->y > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight
						&& pPosOld->y - fHeight < g_aBlock[nCntBlock].pos.y))
				{
					if ((pPosOld->x + fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x
						&& pPos->x + fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;											// 移動量をリセット
						bLand = false;
						bHitHead = false;
					}
					
					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
					&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// 位置をブロックの位置に設定
						pMove->x = 0.0f;																		// 移動量をリセット
						bLand = false;
						bHitHead = false;
					}
				}

				if (bLand == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;								// 移動量をリセット
					}
				}
				else if (bHitHead == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;								// 移動量をリセット
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// 位置をブロックの位置に設定
						pMove->y = 0.0f;							// 移動量をリセット
					}
				}
			}
		}
	}

	return bLand;
}

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int nCntBlock = (EDITBLOCK_NUMBER + 1); nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse != true)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].posOld = pos;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].col = D3DXCOLOR_NULL;
			g_nCounterBlock++;

			g_aBlock[nCntBlock].bUse = true;

			break;
		}
	}
}

BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

int GetCollison(void)
{
	return g_nCheckCollision;
}

int GetSelectNumber(void)
{
	return g_nSelectBlock;
}

//================================================================================================================
// --- 設置したブロックの数の取得 ---
//================================================================================================================
int GetBlockMax(void)
{
	return g_nCounterBlock;
}

//================================================================================================================
// --- ファイルから取得した情報の反映 ---
//================================================================================================================
void SetBlockFromFile(BLOCKFROMEDIT* pbfeBlock)
{
	BLOCK *pBlock = &g_aBlock[0];
	BLOCK *pBlockDoppel = &g_aBlockDoppel[0];

	for (int nCntBlock = EDITBLOCK_NUMBER + 1; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++, pBlockDoppel++)
	{ // 設定
		if (pBlock->bUse == false)
		{ // 使われていなければデータを保存、コピー作成
			pBlock->pos = pBlockDoppel->pos = pbfeBlock->pos;				// 位置
			pBlock->col = pBlockDoppel->col = pbfeBlock->col;				// 色
			pBlock->fWidth = pBlockDoppel->fWidth = pbfeBlock->fWidth;		// 横幅
			pBlock->fHeight = pBlockDoppel->fHeight = pbfeBlock->fHeight;	// 縦幅
			pBlock->bUse = pBlockDoppel->bUse = true;						// 使用済みに変更
			g_nCounterBlock++;												// ポリゴンの数を増やす
			g_nSelectBlock = 1;

			break;
		}
	}
}

//消せるようにする,ブロックの選択,移動速度