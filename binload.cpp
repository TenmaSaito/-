//================================================================================================================
//
// DirectX��.binui�ǂݍ��ݏ��� [read_binui.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "binload.h"

// �O���[�o���ϐ�
BLOCKLOAD g_aBlockLoad[MAX_READ];							// �ǂݍ��񂾏��
int g_nCoutnerBlockLoad;									// �ǂݍ��񂾃u���b�N��
DWORD g_dwLastErrorBlock;									// �ߋ��ɋN�������d��ȃG���[

//================================================================================================================
// binui�ǂݍ��ݏ���
//================================================================================================================
BLOCKLOAD *GetBlockInfo(const char* binPath)
{
	FILE* pFile = NULL;
	HWND hWnd = NULL;
	BLOCKLOAD *pBlockLoad = &g_aBlockLoad[0];
	int nCntBlock = 0;

	g_dwLastErrorBlock = SUCCESS_READ;

	// �O��̎擾��������������
	memset(g_aBlockLoad, NULL, sizeof(g_aBlockLoad));
	g_nCoutnerBlockLoad = 0;

	// �t�@�C���p�X�̒������擾
	if ((int)strlen(binPath) >= MAX_PATH)
	{
#ifdef _DEBUG
		MessageBox(hWnd, "�t�@�C�������������܂��B", "FileError", MB_ICONERROR);
#endif
		g_dwLastErrorBlock = FAILED_TOOLONG;
		return NULL;
	}

	// �t�@�C���I�[�v��
	pFile = fopen(binPath, "rb");

	// �t�@�C���I�[�v������
	if (pFile == NULL)
	{ // �t�@�C���I�[�v�����s��
#ifdef _DEBUG
		MessageBox(hWnd, ".bin�t�@�C�������݂��܂���B", "FileError", MB_ICONERROR);
#endif
		g_dwLastErrorBlock = FAILED_READ;
		return NULL;
	}

	// �|���S���̕`�搔���擾
	fread_s(&nCntBlock, sizeof(int), sizeof(int), 1, pFile);

	g_nCoutnerBlockLoad = nCntBlock;

	// �|���S���̕`�搔���ǂݍ���
	fread(pBlockLoad, sizeof(BLOCKLOAD), nCntBlock, pFile);

	// �擪�A�h���X�ւƏ�����
	pBlockLoad = &g_aBlockLoad[0];

	return pBlockLoad;
}

//================================================================================================================
// �擾�����u���b�N�̑����擾����
//================================================================================================================
int GetBlockLoadNum(void)
{
	return g_nCoutnerBlockLoad;
}

//================================================================================================================
// binload.cpp�ŋN�����Ō�̃G���[�擾����
//================================================================================================================
DWORD GetLastErrorBin(void)
{
	return g_dwLastErrorBlock;
}