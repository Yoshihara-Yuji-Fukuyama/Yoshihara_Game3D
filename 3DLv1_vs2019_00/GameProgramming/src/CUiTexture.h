#ifndef UITEXTUER_H
#define UITEXTURE_H

#include "CCharacter.h"

#define TEXTURE_HP_BAR "HpBar.png"//HP�o�[�摜
#define TEX_HP_MAX 0,625,0,91 //HP�ő�̉摜�����W
#define TEX_HP_1 0,625,104,194//HP-1�̉摜�����W
#define TEX_HP_2 0,625,208,298//HP-2�̉摜�����W
#define TEX_HP_3 0,625,312,402//HP-3�̉摜�����W
#define TEX_HP_4 0,625,415,505//HP-4�̉摜�����W
#define TEX_HP_5 0,625,519,609//HP-5�̉摜�����W
#define TEX_HP_6 0,625,623,713//HP-6�̉摜�����W
#define TEX_HP_7 0,625,726,816//HP-7�̉摜�����W
#define TEX_HP_8 0,625,830,920//HP-8�̉摜�����W
#define TEX_HP_9 0,625,934,1024//HP-9�̉摜�����W
#define TEX_HP_10 0,625,1037,1127//HP-10�̉摜�����W

#define HP_SIZE 312.5f,45.0f //HP�o�[�̑傫��
#define HP_POS 312.5f,45.0f //HP�o�[�̍��W

class CUiTexture : public CCharacter
{
public:
	//UI�̎��
	enum class EUiType
	{
		Hp,
	};
	//�f�t�H���g�R���X�g���N�^
	CUiTexture();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	/// <param name="uiType">UI�̎��</param>
	/// <param name="left">�e�N�X�`���̍�</param>
	/// <param name="right">�e�N�X�`���̉E</param>
	/// <param name="bottom">�e�N�X�`���̉�</param>
	/// <param name="top">�e�N�X�`���̏�</param>
	CUiTexture(float w, float h, EUiType uiType, int left = 0, int right = 0, int bottom = 0, int top = 0);

	//�X�V
	void Update();

	//HP�̕\�����X�V
	void HpBarUpdate();

	//static���\�b�h�̐錾
	static CTexture* GetTextureHpBar();
private:
	//UI�̎��
	EUiType mUiType;
	//���݂�HP
	float mHp;
	//HP�̍ő�l
	float mMaxHp;
	//���̕ۑ�
	float mW;

	//static�ϐ��̐錾
	static CTexture mTextureHpBar;
};
#endif