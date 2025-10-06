//================================================================================================================
//
// DirectX�̃G�f�B�^�p��cpp�t�@�C�� [editer.cpp]
// Author : TENMA
//
//================================================================================================================
#include "editer.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "resource.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
BLOCKFROMEDIT g_aBFEBlock[128];			// �t�@�C�������o���p�G�f�B�^�̏��
bool g_bUseDebugPlay;					// �f�o�b�O�v���C�̏��
bool g_bChanged;						// �������A�������̓��[�h���Ă���l���ύX���ꂽ��
bool g_bChangedSave;					// �ύX��̏������s������

//================================================================================================================
// --- �G�f�B�^�̏����� ---
//================================================================================================================
void InitEditer(void)
{
	g_bChanged = false;

	// �ecpp�̏�����
	InitPlayer();

	InitBlock();
}

//================================================================================================================
// --- �G�f�B�^�̏I�� ---
//================================================================================================================
void UninitEditer(void)
{
	// �ecpp�̏I��
	UninitPlayer();

	UninitBlock();
}

//================================================================================================================
// --- �G�f�B�^�̍X�V ---
//================================================================================================================
void UpdateEditer(void)
{
	HWND hWnd = NULL;
	TCHAR szFileTitle[MAX_TITLE] = {};

	if (FAILED(GetHandleWindow(&hWnd)))
	{
		return;
	}

	// �ecpp�̍X�V
	UpdatePlayer();

	UpdateBlock();

	if (GetKeyboardPress(DIK_LCONTROL) == true
		&& GetKeyboardTrigger(DIK_S) == true)
	{
		strcpy(szFileTitle, GetFileName());
		if (strcmp(szFileTitle, "�Í��j��_�X�e�[�W�G�f�B�^�\�}��") != 0)
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
			if (strcmp(szFileTitle, "�Í��j��_�X�e�[�W�G�f�B�^�\�}��") != 0)
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
// --- �G�f�B�^�̕`�� ---
//================================================================================================================
void DrawEditer(void)
{
	// �ecpp�̕`��
	DrawBlock();

	DrawPlayer();
}

//================================================================================================================
// --- �f�o�b�O�v���C�̐ݒ� ---
//================================================================================================================
void SetEnableDebugPlay(bool bUse)
{
	g_bUseDebugPlay = bUse;
}

//================================================================================================================
// --- �f�[�^�̕ϓ��m�F ---
//================================================================================================================
void SetEnableChanged(bool bChanged)
{
	g_bChanged = bChanged;
	g_bChangedSave = false;
}

//================================================================================================================
// --- �ݒu�����u���b�N�̏��̎擾 ---
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