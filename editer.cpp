//================================================================================================================
//
// DirectXのエディタ用のcppファイル [editer.cpp]
// Author : TENMA
//
//================================================================================================================
#include "editer.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "resource.h"

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
BLOCKFROMEDIT g_aBFEBlock[128];			// ファイル書き出し用エディタの情報
bool g_bUseDebugPlay;					// デバッグプレイの状態
bool g_bChanged;						// 初期化、もしくはロードしてから値が変更されたか
bool g_bChangedSave;					// 変更後の処理を行ったか

//================================================================================================================
// --- エディタの初期化 ---
//================================================================================================================
void InitEditer(void)
{
	g_bChanged = false;

	// 各cppの初期化
	InitPlayer();

	InitBlock();
}

//================================================================================================================
// --- エディタの終了 ---
//================================================================================================================
void UninitEditer(void)
{
	// 各cppの終了
	UninitPlayer();

	UninitBlock();
}

//================================================================================================================
// --- エディタの更新 ---
//================================================================================================================
void UpdateEditer(void)
{
	HWND hWnd = NULL;
	TCHAR szFileTitle[MAX_TITLE] = {};

	if (FAILED(GetHandleWindow(&hWnd)))
	{
		return;
	}

	// 各cppの更新
	UpdatePlayer();

	UpdateBlock();

	if (GetKeyboardPress(DIK_LCONTROL) == true
		&& GetKeyboardTrigger(DIK_S) == true)
	{
		strcpy(szFileTitle, GetFileName());
		if (strcmp(szFileTitle, "暗黒破壊神ステージエディタ―マン") != 0)
		{
			SendMessage(hWnd, WM_COMMAND, ID_UPDATESAVE, NULL);
			EnableMenuItem(GetSubMenu(GetMenu(hWnd), 0), ID_UPDATESAVE, MF_BYCOMMAND | MF_GRAYED);
			strcpy(szFileTitle, GetFileName());
			SetWindowText(hWnd, szFileTitle);
		}
		else
		{
			SendMessage(hWnd, WM_COMMAND, ID_NAMEDSAVE, NULL);
		}
	}

	if (g_bChangedSave == false)
	{
		if (g_bChanged)
		{
			strcpy(szFileTitle, GetFileName());
			if (strcmp(szFileTitle, "暗黒破壊神ステージエディタ―マン") != 0)
			{
				EnableMenuItem(GetSubMenu(GetMenu(hWnd), 0), ID_UPDATESAVE, MF_BYCOMMAND | MF_ENABLED);
				strcat(szFileTitle, "*");
				SetWindowText(hWnd, szFileTitle);
			}

			g_bChangedSave = true;
		}
		else
		{
			EnableMenuItem(GetSubMenu(GetMenu(hWnd), 0), ID_UPDATESAVE, MF_BYCOMMAND | MF_GRAYED);
			strcpy(szFileTitle, GetFileName());
			SetWindowText(hWnd, szFileTitle);
			g_bChangedSave = true;
		}
	}
}

//================================================================================================================
// --- エディタの描画 ---
//================================================================================================================
void DrawEditer(void)
{
	// 各cppの描画
	DrawBlock();

	DrawPlayer();
}

//================================================================================================================
// --- デバッグプレイの設定 ---
//================================================================================================================
void SetEnableDebugPlay(bool bUse)
{
	g_bUseDebugPlay = bUse;
}

//================================================================================================================
// --- データの変動確認 ---
//================================================================================================================
void SetEnableChanged(bool bChanged)
{
	g_bChanged = bChanged;
	g_bChangedSave = false;
}

//================================================================================================================
// --- 設置したブロックの情報の取得 ---
//================================================================================================================
BLOCKFROMEDIT *GetEditerInfo(void)
{
	int nCntEditerMax = GetBlockMax();
	BLOCK *pBlock = GetBlock();

	for (int nCntEditer = 0; nCntEditer < nCntEditerMax; nCntEditer++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			g_aBFEBlock[nCntEditer].pos = pBlock->pos;
			g_aBFEBlock[nCntEditer].col = pBlock->col;
			g_aBFEBlock[nCntEditer].fWidth = pBlock->fWidth;
			g_aBFEBlock[nCntEditer].fHeight = pBlock->fHeight;
			g_aBFEBlock[nCntEditer].bUse = true;
		}
	}

	return &g_aBFEBlock[0];
}