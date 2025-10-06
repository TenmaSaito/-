//================================================================================================================
//
// DirectX�̃u���b�N�̐ݒu�p��cpp�t�@�C�� [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "block.h"
#include "input.h"

//*************************************************************************************************
//*** �}�N����` ***
//*************************************************************************************************
#define MAX_BLOCK		(128)		// �u���b�N�̍ő吔

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK] = {};					// �u���b�N�̏��
int g_nCheckCollision;

//================================================================================================================
// --- �u���b�N�̏����� ---
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	VERTEX_2D* pVtx = NULL;							// ���_���ւ̃|�C���^
	int nCntBlock = 0;
	g_nCheckCollision = 0;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(600.0f, 450.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		g_aBlock[nCntBlock].fHeight = 150.0f;
		g_aBlock[nCntBlock].fWidth = 200.0f;
		g_aBlock[nCntBlock].bUse = false;
	}

	SetBlock(D3DXVECTOR3(200.0f, 670.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 620.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 570.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 520.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(200.0f, 470.0f, 0.0f), 50.0f, 50.0f);

	/*SetBlock(D3DXVECTOR3(300.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(350.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(400.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(450.0f, 710.0f, 0.0f), 50.0f, 50.0f);
	SetBlock(D3DXVECTOR3(500.0f, 710.0f, 0.0f), 50.0f, 50.0f);*/

	/*** �e�N�X�`���̓ǂݍ��� ***/
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block.png",
		&g_pTextureBlock);

	/*** ���_�o�b�t�@�̐��� ***/
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		/*** ���_���W�̐ݒ�̐ݒ� ***/
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

		/*** rhw�̐ݒ� ***/
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		/*** ���_�J���[�ݒ� ***/
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		/*** �e�N�X�`�����W�̐ݒ� ***/
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- �u���b�N�̏I������ ---
//================================================================================================================
void UninitBlock(void)
{
	/*** �e�N�X�`���̔j�� ***/
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	/*** ���_�o�b�t�@�̔j�� ***/
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// --- �u���b�N�̍X�V���� ---
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
		}
	}

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardRepeat(DIK_O))
	{
		g_aBlock[0].fHeight += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_U))
	{
		g_aBlock[0].fHeight -= 1.0f;
	}

	if (GetKeyboardRepeat(DIK_M))
	{
		g_aBlock[0].fWidth += 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_N))
	{
		g_aBlock[0].fWidth -= 1.0f;
	}

	if (GetKeyboardRepeat(DIK_I))
	{
		g_aBlock[0].pos.y -= 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_K))
	{
		g_aBlock[0].pos.y += 1.0f;
	}

	if (GetKeyboardRepeat(DIK_J))
	{
		g_aBlock[0].pos.x -= 1.0f;
	}
	else if (GetKeyboardRepeat(DIK_L))
	{
		g_aBlock[0].pos.x += 1.0f;
	}

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** ���_���W�̐ݒ�̐ݒ� ***/
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
		}

		pVtx += 4;
	}

	/*** ���_�o�b�t�@�̐ݒ���I�� ***/
	g_pVtxBuffBlock->Unlock();
}

//================================================================================================================
// --- �u���b�N�̕`�揈�� ---
//================================================================================================================
void DrawBlock(void)
{
	/*** �f�o�C�X�̎擾 ***/
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	/*** ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ� ***/
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	/*** ���_�t�H�[�}�b�g�̐ݒ� ***/
	pDevice->SetFVF(FVF_VERTEX_2D);

	/*** �e�N�X�`���̐ݒ� ***/
	pDevice->SetTexture(0, g_pTextureBlock);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			/*** �|���S���̕`�� ***/
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntBlock,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// --- �u���b�N�̓����蔻�菈�� ---
//================================================================================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, OR_GRAVITY gravity)
{
	bool bLand = false;					// �󒆂ɕ�����ł��锻��ŏ�����
	bool bHitHead = false;
	bool bMove[4] = { false };
	D3DXVECTOR3 posBefore = *pPos;		// �����O�̈ʒu

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
						bLand = true;							// ���n�����
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
						bLand = true;								// ���n�����
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
						pPos->x = g_aBlock[nCntBlock].pos.x - fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;											// �ړ��ʂ����Z�b�g
						bLand = false;
						bHitHead = false;
					}
					
					if ((pPosOld->x - fWidth * 0.5f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth
					&& pPos->x - fWidth * 0.5f <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth))
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + fWidth * 0.5f;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->x = 0.0f;																		// �ړ��ʂ����Z�b�g
						bLand = false;
						bHitHead = false;
					}
				}

				if (bLand == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;								// �ړ��ʂ����Z�b�g
					}
				}
				else if (bHitHead == true)
				{
					if (gravity == OR_GRAVITY_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + fHeight;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;								// �ړ��ʂ����Z�b�g
					}

					if (gravity == OR_GRAVITY_ANTI_GRAVITY)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y;		// �ʒu���u���b�N�̈ʒu�ɐݒ�
						pMove->y = 0.0f;							// �ړ��ʂ����Z�b�g
					}
				}
			}
		}
	}

	return bLand;
}

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse != true)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;

			g_aBlock[nCntBlock].bUse = true;

			break;
		}
	}
}

BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

void CheckBlockMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fHeight, float fWidth)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			
		}
	}
}

int GetCollison(void)
{
	return g_nCheckCollision;
}