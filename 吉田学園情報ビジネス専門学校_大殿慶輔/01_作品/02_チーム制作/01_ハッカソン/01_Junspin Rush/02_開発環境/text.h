//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _TEXT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
#define TEXT_MAX	(128)	// �e�L�X�g�̍ő吔

//****************************************
// �񋓌^�̒�`
//****************************************

// �e�L�X�g�̎g�p�̈�
typedef enum
{
	TEXT_FIELD_UI = 0,	// UI
	TEXT_FIELD_DAMAGE,	// �_���[�W�\��
	TEXT_FIELD_MENU,	// ���j���[
	TEXT_FIELD_MAX,
}TEXT_FIELD;

// �e�L�X�g�̃��[�h
typedef enum 
{
	TEXT_MODE_LEFT = 0,	// �E�l��
	TEXT_MODE_RIGHT,	// ���l��
	TEXT_MODE_CENTER,	// ���S
	TEXT_MODE_MAX
}TEXT_MODE;

// �e�L�X�g�̃t�H���g
typedef enum
{
	FONT_000 = 0,	// �t�H���g[000]
	FONT_001,		// �t�H���g[001]
	FONT_002,		// �t�H���g[002]
	FONT_003,		// �t�H���g[003]
	FONT_004,		// �t�H���g[004]
	FONT_MAX
}FONT;

//****************************************
// �\���̂̒�`
//****************************************

// �e�L�X�g�̏��
typedef struct 
{
	char		aDisplay[TXT_MAX];	// �\���`��
	D3DXVECTOR3 pos;				// �ʒu
	Color		col;				// �F
	FONT		font;				// �t�H���g
}Text;

// �t�H���g���̍\���̂��`
typedef struct
{
	char aPath[TXT_MAX];	// �e�N�X�`���t�@�C���̑��΃p�X
	float fWidth;			// ��
	float fHeight;			// ����
	float fSpaceX;			// �z�u�ԊuX
	float fSpaceY;			// �z�u�ԊuY
	char cStartChar;		// �J�n����
	int nPtnMaxX;			// �e�L�X�g�̃p�^�[����(��)
	int nPtnMaxY;			// �e�L�X�g�̃p�^�[����(�c)
}Font;

//****************************************
// �v���g�^�C�v�錾
//****************************************
Font *GetFont(void);	// �t�H���g�̏����擾
void InitTxt_00		(void);																																			// �e�L�X�g�̏���������
void UninitTxt_00	(void);																																			// �e�L�X�g�̏I������
void UpdateTxt_00	(TEXT_FIELD textField, int nNumber, char aString[TXT_MAX], D3DXVECTOR3 pos, Color col, TEXT_MODE mode, FONT font, float fWidth, float fHeight);	// �e�L�X�g�̍X�V����
void DrawTxt_00		(void);																																			// �e�L�X�g�̕`�揈��
void ShowTxt_00		(TEXT_FIELD textField, int nNumber, bool bShow);																								// �e�L�X�g�̕\��/��\����؂�ւ���

#endif