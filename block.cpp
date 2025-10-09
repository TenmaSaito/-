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
#define EDITBLOCK_UP				DIK_I					// �G�f�B�^�u���b�N����ɂ��炷
#define EDITBLOCK_DOWN				DIK_K					// �G�f�B�^�u���b�N�����ɂ��炷
#define EDITBLOCK_LEFT				DIK_J					// �G�f�B�^�u���b�N�����ɂ��炷
#define EDITBLOCK_RIGHT				DIK_L					// �G�f�B�^�u���b�N���E�ɂ��炷
#define EDITBLOCK_WIDTH_UP			DIK_M					// �G�f�B�^�u���b�N���E�ɐL�΂�
#define EDITBLOCK_WIDTH_DOWN		DIK_N					// �G�f�B�^�u���b�N�����ɏk�߂�
#define EDITBLOCK_HEIGHT_UP			DIK_O					// �G�f�B�^�u���b�N�����ɐL�΂�
#define EDITBLOCK_HEIGHT_DOWN		DIK_U					// �G�f�B�^�u���b�N����ɏk�߂�
#define SELECTBLOCK_UP				DIK_T					// �I�����Ă���u���b�N����ɂ��炷
#define SELECTBLOCK_DOWN			DIK_G					// �I�����Ă���u���b�N�����ɂ��炷
#define SELECTBLOCK_LEFT			DIK_F					// �I�����Ă���u���b�N�����ɂ��炷
#define SELECTBLOCK_RIGHT			DIK_H					// �I�����Ă���u���b�N���E�ɂ��炷
#define SELECTBLOCK_NEXT			DIK_UP					// �I��ԍ�������ɂ��炷
#define SELECTBLOCK_LAST			DIK_DOWN				// �I��ԍ�����O�ɖ߂�
#define EDITBLOCK_CREATE			DIK_RETURN				// �G�f�B�^�u���b�N�̈ʒu�ɃG�f�B�^�u���b�N�̏�Ԃ̃u���b�N��z�u����
#define SELECTBLOCK_DELETE			DIK_BACK				// �I�����Ă���u���b�N������

//*************************************************************************************************
//*** �O���[�o���ϐ� ***
//*************************************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK] = {};					// �u���b�N�̏��
BLOCK g_aBlockDoppel[MAX_BLOCK] = {};			// �u���b�N�̏��̃R�s�[(�f�[�^�̊Ǘ��Ɏg�p ����{�I�Ɏg�p���Ȃ����ƁI)
int g_nCheckCollision;
int g_nSelectBlock;								//�I�𒆂̃u���b�N
int g_nCounterBlock;							//�������ꂽ�u���b�N�̐�
bool g_bChangedBlock;							// �u���b�N�̏�񂪕ω�������
int g_nblinkCounter;							//�_�ŃJ�E���^
bool g_bBlink;									//�_�Ő؂�ւ�

//================================================================================================================
// --- �u���b�N�̏����� ---
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	VERTEX_2D* pVtx = NULL;							// ���_���ւ̃|�C���^
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
		pVtx[0].col = g_aBlock[nCntBlock].col;
		pVtx[1].col = g_aBlock[nCntBlock].col;
		pVtx[2].col = g_aBlock[nCntBlock].col;
		pVtx[3].col = g_aBlock[nCntBlock].col;

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

	bool bUseDebugPlay = GetDebugPlay();		// �f�o�b�O�v���C���L�����擾

	if (bUseDebugPlay == true)
	{// �f�o�b�O�v���C��
		g_aBlock[EDITBLOCK_NUMBER].bUse = false;	// �I���u���b�N�̖�����
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

	/*** ���_�o�b�t�@�̐ݒ� ***/
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	if (bUseDebugPlay != true)
	{// �f�o�b�O�v���C���łȂ��Ȃ�
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
						//����̃u���b�N�����i�[
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

	//�����O�u���b�N�_��
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
		// �R�s�[�f�[�^�ƌ���ׂāA�ω����Ȃ����m�F
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

			if (bUseDebugPlay == true)
			{// �f�o�b�O�v���C��
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
// --- �ݒu�����u���b�N�̐��̎擾 ---
//================================================================================================================
int GetBlockMax(void)
{
	return g_nCounterBlock;
}

//================================================================================================================
// --- �t�@�C������擾�������̔��f ---
//================================================================================================================
void SetBlockFromFile(BLOCKFROMEDIT* pbfeBlock)
{
	BLOCK *pBlock = &g_aBlock[0];
	BLOCK *pBlockDoppel = &g_aBlockDoppel[0];

	for (int nCntBlock = EDITBLOCK_NUMBER + 1; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++, pBlockDoppel++)
	{ // �ݒ�
		if (pBlock->bUse == false)
		{ // �g���Ă��Ȃ���΃f�[�^��ۑ��A�R�s�[�쐬
			pBlock->pos = pBlockDoppel->pos = pbfeBlock->pos;				// �ʒu
			pBlock->col = pBlockDoppel->col = pbfeBlock->col;				// �F
			pBlock->fWidth = pBlockDoppel->fWidth = pbfeBlock->fWidth;		// ����
			pBlock->fHeight = pBlockDoppel->fHeight = pbfeBlock->fHeight;	// �c��
			pBlock->bUse = pBlockDoppel->bUse = true;						// �g�p�ς݂ɕύX
			g_nCounterBlock++;												// �|���S���̐��𑝂₷
			g_nSelectBlock = 1;

			break;
		}
	}
}

//������悤�ɂ���,�u���b�N�̑I��,�ړ����x