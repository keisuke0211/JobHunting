//========================================
// 
// �G�t�F�N�g�S��[00] �̏���
// Author:���� ����
// 
//========================================
// *** eff_general_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "sound.h"
#include "physics.h"
#include "polygon.h"
#include "eff_general_00.h"
#include <stdio.h>
#include <float.h>

//****************************************
// �}�N����`
//****************************************

// �G�t�F�N�g�S��[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
#define EFF_GENERAL_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFF_GENERAL_00_TYPE_DATA.txt"

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameter(Eff_general_00 *pEff);

// �p�����[�^�[����ނɉ����Đݒ�
void SetParameter(Eff_general_00 *pEff);

// �T�E���h�ǂݍ��ݏ���
void SoundLoading(Eff_general_00 *pEff);

// �p�^�[��No.�ǂݍ��ݏ���
void PatternLoading(Eff_general_00 *pEff);

// �����ǂݍ��ݏ���
void SummonLoading(Eff_general_00 *pEff);

// ��������
void Summon(Eff_general_00 *pEff, int nCntSummon);

// �e�̒ǐՏ���
void ParentTrackingProcess(Eff_general_00 *pEff);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_general_00	// �e�N�X�`���ւ̃|�C���^
						[EFF_GENERAL_00_TYPE_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_general_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Eff_general_00			g_aEff_general_00			// �G�t�F�N�g�S��[00] �̏��
						[EFF_GENERAL_00_MAX];		//
Eff_general_00Type		g_aEff_general_00Type		// �G�t�F�N�g�S��[00] �̎�ޖ��̏��
						[EFF_GENERAL_00_TYPE_MAX];	//

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameter�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameter(Eff_general_00 *pEff) 
{
	pEff->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pEff->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pEff->targetRot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW����
	pEff->fWidth			= 0.0f;								// ��
	pEff->fHeight			= 0.0f;								// ����
	pEff->nType				= 0;								// ���
	pEff->bUse				= false;							// �g�p����Ă��邩�t���O
	pEff->nLife				= 0;								// ����
	pEff->nPattern			= 0;								// �p�^�[��No.
	pEff->nCounterAnim		= 0;								// �A�j���[�V�����̃J�E���^�[
	pEff->nCounterSound		= 0;								// �T�E���h�J�E���^�[
	pEff->nCounterPattern	= 0;								// �p�^�[��No.�J�E���^�[
	pEff->nCounterSummon	= 0;								// �����J�E���^�[
	pEff->bDraw				= true;								// �`��t���O
}

//========================================
// SetParameter�֐� - �p�����[�^�[����ނɉ����Đݒ� -
//========================================
void SetParameter(Eff_general_00 *pEff)
{
	pEff->fWidth	= g_aEff_general_00Type[pEff->nType].fWidth;	// ��
	pEff->fHeight	= g_aEff_general_00Type[pEff->nType].fHeight;	// ����
	pEff->nLife		= g_aEff_general_00Type[pEff->nType].nLife;		// ����
}

//========================================
// SoundLoading�֐� - �T�E���h�ǂݍ��ݏ��� -
//========================================
void SoundLoading(Eff_general_00 *pEff)
{
	for (int nCntSound = 0; nCntSound < g_aEff_general_00Type[pEff->nType].nSoundNum; nCntSound++)
	{
		if (pEff->nCounterSound == g_aEff_general_00Type[pEff->nType].aSound[nCntSound].nTime)
		{// �T�E���h�J�E���^�[���T�E���h��炷���Ԃƈ�v�������A
			// �T�E���h���Đ�
			PlaySound(g_aEff_general_00Type[pEff->nType].aSound[nCntSound].soundLabel);
		}
	}

	if ((pEff->nCounterSound >= g_aEff_general_00Type[pEff->nType].nSoundLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nSoundLoopTime != -1))
	{// �T�E���h�J�E���^�[���T�E���h�̃��[�v���ԂɒB�������A���T�E���h�̃��[�v���Ԃ�-1�łȂ����A
		// �T�E���h�J�E���^�[��������
		pEff->nCounterSound = -1;
	}

	// �T�E���h�J�E���^�[�����Z
	pEff->nCounterSound++;
}

//========================================
// PatternLoading�֐� - �p�^�[��No.�ǂݍ��ݏ��� -
//========================================
void PatternLoading(Eff_general_00 *pEff)
{
	for (int nCntPattern = 0; nCntPattern < g_aEff_general_00Type[pEff->nType].nPatternNum; nCntPattern++)
	{
		if (pEff->nCounterPattern == g_aEff_general_00Type[pEff->nType].aPattern[nCntPattern].nTime)
		{// �p�^�[��No.�J�E���^�[���p�^�[��No.��ύX���鎞�Ԃƈ�v�������A
			// �p�^�[��No.���X�V
			pEff->nPattern = g_aEff_general_00Type[pEff->nType].aPattern[nCntPattern].nPattern;

			if (pEff->nPattern == -1) 
			{// �p�^�[��No.��-1���������A
				// �`��t���O���U�ɂ���
				pEff->bDraw = false;
			}
			else 
			{// �p�^�[��No.��-1�łȂ����A
				// �`��t���O��^�ɂ���
				pEff->bDraw = true;
			}
		}
	}

	if ((pEff->nCounterPattern >= g_aEff_general_00Type[pEff->nType].nPatternLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nPatternLoopTime != -1))
	{// �p�^�[��No.�J�E���^�[���p�^�[��No.�̃��[�v���ԂɒB�������A���p�^�[��No.�̃��[�v���Ԃ�-1�łȂ����A
		// �p�^�[��No.�J�E���^�[��������
		pEff->nCounterPattern = -1;
	}

	// �p�^�[��No.�J�E���^�[�����Z
	pEff->nCounterPattern++;
}

//========================================
// SummonLoading�֐� - �����ǂݍ��ݏ��� -
//========================================
void SummonLoading(Eff_general_00 *pEff)
{
	for (int nCntSummon = 0; nCntSummon < g_aEff_general_00Type[pEff->nType].nSummonNum; nCntSummon++)
	{
		if (pEff->nCounterSummon == g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].nTime)
		{// �����J�E���^�[���������鎞�Ԃƈ�v�������A
			// ��������
			Summon(pEff, nCntSummon);
		}
	}

	if ((pEff->nCounterSummon >= g_aEff_general_00Type[pEff->nType].nSummonLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nSummonLoopTime != -1))
	{// �����J�E���^�[�������̃��[�v���ԂɒB�������A�������̃��[�v���Ԃ�-1�łȂ����A
		// �����J�E���^�[��������
		pEff->nCounterSummon = -1;
	}

	// �����J�E���^�[�����Z
	pEff->nCounterSummon++;
}

//========================================
// Summon�֐� - �������� -
//========================================
void Summon(Eff_general_00 *pEff, int nCntSummon) 
{
	D3DXVECTOR3 setPos			//
				= pEff->pos;	// �ݒ�ʒu

	// �ݒ�ʒu��ݒ�
	setPos.x += sinf(pEff->rot.z - D3DX_PI * 0.5f)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.x;
	setPos.y += cosf(pEff->rot.z - D3DX_PI * 0.5f)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.x;
	setPos.x += sinf(pEff->rot.z + D3DX_PI)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.y;
	setPos.y += cosf(pEff->rot.z + D3DX_PI)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.y;

	// �G�t�F�N�g�S��[00] �̐ݒ菈��
	SetEff_general_00(
		setPos,
		D3DXVECTOR3(
			0.0f,
			0.0f,
			pEff->rot.z + g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].fAngle),
		g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].nEnemyType,
		false, 
		NULL,
		NULL);
}

//========================================
// ParentTrackingProcess�֐�- �e�̒ǐՏ��� -
//========================================
void ParentTrackingProcess(Eff_general_00 *pEff)
{
	if (!pEff->bChild)
	{// �q�t���O���U�̎��A
		// �������I������
		return;
	}

	// �ʒu���X�V
	pEff->pos += *pEff->pParentPos - pEff->parentPosTemp;

	D3DXVECTOR3 rot;											// �e����q�ւ̌���
	float		fDistance										// 
				= FindDistance(*pEff->pParentPos, pEff->pos);	// �e�Ǝq�̋���

	// �e����q�ւ̌�����ݒ�
	rot.z = FindAngle(*pEff->pParentPos, pEff->pos) + (pEff->pParentRot->z - pEff->parentRotTemp.z);

	// �q�Ɍ����̍�����K�p
	pEff->rot.z += (pEff->pParentRot->z - pEff->parentRotTemp.z);

	// �ʒu��e�̏��ɖ߂�����A
	// �������p�x�ɋ��������Z����
	pEff->pos = *pEff->pParentPos;
	pEff->pos.x += sinf(rot.z) * fDistance;
	pEff->pos.y += cosf(rot.z) * fDistance;

	// �e�̈ʒu�Ɗp�x��ۑ�
	pEff->parentPosTemp = *pEff->pParentPos;
	pEff->parentRotTemp = *pEff->pParentRot;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_general_00�֐� - �G�t�F�N�g�S��[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadEff_general_00(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	Eff_general_00Type	*pEffType	// �G�t�F�N�g�S��[00] �̎�ޖ��̏��
						= g_aEff_general_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(EFF_GENERAL_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �������ʎq��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (strcmp(aDataSearch, "END") == 0)
		{// �t�@�C�������
			fclose(pFile);
			break;
		}

		if (aDataSearch[0] == '#')
		{// �R�����g��ƈ�v�������A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (strcmp(aDataSearch, "EFF_GENERAL_00_TYPE") == 0)
		{// �G�t�F�N�g�S��[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n

			// ��������������
			pEffType->nSummonNum = 0;

			// �T�E���h����������
			pEffType->nSoundNum = 0;

			// �p�^�[��No.����������
			pEffType->nPatternNum = 0;

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (strcmp(aDataSearch, "EFF_GENERAL_00_TYPE_END") == 0)
				{// �G�t�F�N�g�S��[00] �̎�ޖ��̏��̓ǂݍ��݂��I��
					pEffType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "�ǂݍ��ݖ�:") == 0)
				{// �ǂݍ��ݖ���ǂݍ���
					fscanf(pFile, "%s", pEffType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "�e�N�X�`���̑��΃p�X:") == 0)
				{// �e�N�X�`���̑��΃p�X��ǂݍ���
					fscanf(pFile, "%s", pEffType->aTexturPath);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��X:") == 0)
				{// �p�^�[�����X��ǂݍ���
					fscanf(pFile, "%d", &pEffType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̉���:") == 0)
				{// �A�j���p�^�[���̉�����ǂݍ���
					fscanf(pFile, "%d", &pEffType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̏��:") == 0)
				{// �A�j���p�^�[���̏����ǂݍ���
					fscanf(pFile, "%d", &pEffType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��Y:") == 0)
				{// �p�^�[�����Y��ǂݍ���
					fscanf(pFile, "%d", &pEffType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎���:") == 0)
				{// �A�j���[�V�����ɂ����鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pEffType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎��:") == 0)
				{// �A�j���[�V�����̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �A�j���[�V�����̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadAnimType(aDataSearch, &pEffType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "��:") == 0)
				{// ����ǂݍ���
					fscanf(pFile, "%f", &pEffType->fWidth);

					// �����s�N�Z���P�ʂŏC��
					pEffType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "����:") == 0)
				{// ������ǂݍ���
					fscanf(pFile, "%f", &pEffType->fHeight);

					// �������s�N�Z���P�ʂŏC��
					pEffType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����̎��:") == 0)
				{// �����̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �����̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadAngleType(aDataSearch, &pEffType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "����:") == 0)
				{// ������ǂݍ���
					fscanf(pFile, "%d", &pEffType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "SOUND") == 0)
				{// �T�E���h�̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "SOUND_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �T�E���h�̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->nSoundLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "�T�E���h:") == 0)
						{// �T�E���h��炷���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->aSound[pEffType->nSoundNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							// �T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
							StringLoadSound(aDataSearch, &pEffType->aSound[pEffType->nSoundNum].soundLabel);

							// �T�E���h�������Z
							pEffType->nSoundNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "PATTERN") == 0)
				{// �p�^�[��No.�̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "PATTERN_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �p�^�[��No.�̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->nPatternLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "�p�^�[��:") == 0)
						{// �p�^�[��No.��ύX���鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->aPattern[pEffType->nPatternNum].nTime);

							// �p�^�[��No.��ǂݍ���
							fscanf(pFile, "%d", &pEffType->aPattern[pEffType->nPatternNum].nPattern);

							// �p�^�[��No.�������Z
							pEffType->nPatternNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "SUMMON") == 0)
				{// ���˂̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "SUMMON_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �����̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->nSummonLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "����:") == 0)
						{// �������鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							if (strcmp(aDataSearch, "[") == 0)
							{
								// ��������e�ԍ��Ǝq�ԍ���������
								pEffType->aSummon[pEffType->nSummonNum].nParentNum	= -1;
								pEffType->aSummon[pEffType->nSummonNum].nChildNum	= -1;

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // ����

									if (strcmp(aDataSearch, "]") == 0)
									{// �J��Ԃ������𔲂���
										break;
									}//====================//
									else if (strcmp(aDataSearch, "���:") == 0)
									{// �G�t�F�N�g�S�ʂ̎�ނ�ǂݍ���
										fscanf(pFile, "%s", aDataSearch); // ����

										// �G�t�F�N�g�S�ʂ̎�ނ𕶎��񂩂�ǂݍ���
										StringLoadEff_general_00Type(aDataSearch, &pEffType->aSummon[pEffType->nSummonNum].nEnemyType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// ��������ʒuX��ǂݍ���
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// ��������ʒuY��ǂݍ���
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "�p�x:") == 0)
									{// ��������p�x��ǂݍ���
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].fAngle);

										// �p�x�𐳂����l�ɕϊ�
										pEffType->aSummon[pEffType->nSummonNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "�e:") == 0)
									{// �e�ԍ���ǂݍ���
										fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nParentNum);
									}//====================//
									else if (strcmp(aDataSearch, "�q:") == 0)
									{// �q���ԍ���ǂݍ���
										fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nChildNum);
									}//====================//
								}
							}

							// �����������Z
							pEffType->nSummonNum+=1;
						}
					}
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadEff_general_00Type�֐� - �G�t�F�N�g�S��[00] �̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadEff_general_00Type(char aString[TXT_MAX], int *pType)
{
	Eff_general_00Type	*pEffType					//
						= g_aEff_general_00Type;	// �G�t�F�N�g�S��[00] �̎�ޖ��̏��

	for (int nCntChrType = 0; nCntChrType < EFF_GENERAL_00_TYPE_MAX; nCntChrType++, pEffType++)
	{
		if (strcmp(aString, pEffType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �G�t�F�N�g�S�ʂ̎�ނ���
			*pType = nCntChrType;
			break;
		}
	}
}

//========================================
// InitEff_general_00�֐� - �G�t�F�N�g�S��[00] �̏��������� -
//========================================
void InitEff_general_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntEff_general_00Type = 0; nCntEff_general_00Type < EFF_GENERAL_00_TYPE_MAX; nCntEff_general_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_general_00Type[nCntEff_general_00Type].aTexturPath, &g_aTextureEff_general_00[nCntEff_general_00Type]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * EFF_GENERAL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_general_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff	// �G�t�F�N�g�S��[00] �̏��̃|�C���^
					= g_aEff_general_00;

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pVtx+=4, pEff++)
	{
		// �p�����[�^�[�����������A��ނɉ����Đݒ�
		InitParameter(pEff);
		SetParameter(pEff);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);

		// rhw�̐ݒ�
		SetRHW(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			pEff->nPattern,
			g_aEff_general_00Type[pEff->nType].nPtnMaxX,
			g_aEff_general_00Type[pEff->nType].nPtnMaxY,
			NULL);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_general_00->Unlock();
}

//========================================
// UninitEff_general_00�֐� - �G�t�F�N�g�S��[00] �̏I������ -
//========================================
void UninitEff_general_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEff_general_00 = 0; nCntEff_general_00 < EFF_GENERAL_00_TYPE_MAX; nCntEff_general_00++)
	{
		if (g_aTextureEff_general_00[nCntEff_general_00] != NULL)
		{
			g_aTextureEff_general_00[nCntEff_general_00]->Release();
			g_aTextureEff_general_00[nCntEff_general_00] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_general_00 != NULL) 
	{
		g_pVtxBuffEff_general_00->Release();
		g_pVtxBuffEff_general_00 = NULL;
	}
}

//========================================
// UpdateEff_general_00�֐� - �G�t�F�N�g�S��[00] �̍X�V���� -
//========================================
void UpdateEff_general_00(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff	// �G�t�F�N�g�S��[00] �̏��̃|�C���^
					= g_aEff_general_00;

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pEff++, pVtx+=4)
	{
		if (!pEff->bUse)
		{// �g�p����Ă��Ȃ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// ���������Z�������ʐs�������A
			// �g�p���Ă��Ȃ���Ԃɂ���
			pEff->bUse = false;

			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �e�̒ǐՏ���
		ParentTrackingProcess(pEff);

		// �T�E���h�ǂݍ��ݏ���
		SoundLoading(pEff);

		// �p�^�[��No.�ǂݍ��ݏ���
		PatternLoading(pEff);

		// �����ǂݍ��ݏ���
		SummonLoading(pEff);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&pEff->nPattern,
				g_aEff_general_00Type[pEff->nType].nAnimPtnMin,
				g_aEff_general_00Type[pEff->nType].nAnimPtnMax,
				&pEff->nCounterAnim,
				g_aEff_general_00Type[pEff->nType].nAnimTime,
				g_aEff_general_00Type[pEff->nType].animType),
			g_aEff_general_00Type[pEff->nType].nPtnMaxX,
			g_aEff_general_00Type[pEff->nType].nPtnMaxY,
			NULL);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_general_00->Unlock();
}

//========================================
// DrawEff_general_00�֐� - �G�t�F�N�g�S��[00] �̕`�揈�� -
//========================================
void DrawEff_general_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_general_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Eff_general_00	*pEff					//
					= g_aEff_general_00;	// �G�t�F�N�g�S��[00] �̏��̃|�C���^

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pEff++)
	{
		if ((pEff->bUse)
			&& (pEff->bDraw))
		{// �g�p����Ă����ԁA���`��t���O���^�̎��A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureEff_general_00[pEff->nType]);

			// �G�t�F�N�g�S��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChr * 4, 2);
		}
	}
}

//========================================
// SetEff_general_00�֐� - �G�t�F�N�g�S��[00] �̐ݒ菈�� -
//========================================
void SetEff_general_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bParent, D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pParentRot)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff					//
					= g_aEff_general_00;	// �G�t�F�N�g�S��[00] �̏��̃|�C���^

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pVtx += 4, pEff++)
	{
		if (pEff->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �p�����[�^������������
		InitParameter(pEff);

		pEff->pos		= pos;		// �ʒu����
		pEff->rot		= rot;		// �p�x����
		pEff->nType		= nType;	// ��ނ���
		pEff->bChild	= bParent;	// �e�q�t���O����

		if (bParent) 
		{// �e�q�t���O���^�̎��A
			// �e�̈ʒu�ƌ����̃|�C���^����
			pEff->pParentPos = pParentPos;
			pEff->pParentRot = pParentRot;

			// �e�̈ʒu�ƌ�����ۑ�
			pEff->parentPosTemp = *pParentPos;
			pEff->parentRotTemp = *pParentRot;
		}

		// �p�����[�^����ނɉ����Đݒ�
		SetParameter(pEff);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);

		// �g�p���Ă����Ԃɂ���
		pEff->bUse = true;

		break;
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_general_00->Unlock();
}