#include "CUiTexture.h"
#include "CXPlayer.h"


//static�ϐ��̒�`
CTexture CUiTexture::mTextureHpBar;

//static���\�b�h�̒�`
CTexture* CUiTexture::GetTextureHpBar()
{
	return &mTextureHpBar;
}

CUiTexture::CUiTexture()
{
}

CUiTexture::CUiTexture(float w, float h, EUiType uiType, int left, int right, int bottom, int top)
	: CUiTexture()
{
	SetW(w);
	SetH(h);
	mW = w;

	mUiType = uiType;
	//HP�̍ő�l��ݒ�
	mMaxHp = CXPlayer::GetInstance()->GetHp();
	if (mUiType == EUiType::Hp)
	{
		Texture(GetTextureHpBar(), TEX_HP_MAX);
	}
}

//�X�V
void CUiTexture::Update()
{
	//���݂�HP��ݒ�
	mHp = CXPlayer::GetInstance()->GetHp();

	HpBarUpdate();//HP�\�����X�V

	//HP�o�[�̍��W��ݒ�
	if (mUiType == EUiType::Hp)
	{
		Set(HP_POS, GetW(), GetH());
	}
}

//HP�\�����X�V
void CUiTexture::HpBarUpdate()
{
	//HP�̊������v�Z
	float mHpPar = mHp / mMaxHp;

	if (mHpPar >= 1.0f)//0������
	{
		Texture(GetTextureHpBar(), TEX_HP_MAX);
	}
	else if (mHpPar >= 0.9f)//1������
	{
		Texture(GetTextureHpBar(), TEX_HP_1);
	}
	else if (mHpPar >= 0.8f)//2������
	{
		Texture(GetTextureHpBar(), TEX_HP_2);
	}
	else if (mHpPar >= 0.7f)//3������
	{
		Texture(GetTextureHpBar(), TEX_HP_3);
	}
	else if (mHpPar >= 0.6f)//4������
	{
		Texture(GetTextureHpBar(), TEX_HP_4);
	}
	else if (mHpPar >= 0.5f)//5������
	{
		Texture(GetTextureHpBar(), TEX_HP_5);
	}
	else if (mHpPar >= 0.4f)//6������
	{
		Texture(GetTextureHpBar(), TEX_HP_6);
	}
	else if (mHpPar >= 0.3f)//7������
	{
		Texture(GetTextureHpBar(), TEX_HP_7);
	}
	else if (mHpPar >= 0.2f)//8������
	{
		Texture(GetTextureHpBar(), TEX_HP_8);
	}
	else if (mHpPar >= 0.1f)//9������
	{
		Texture(GetTextureHpBar(), TEX_HP_9);
	}
	else //10������
	{
		Texture(GetTextureHpBar(), TEX_HP_10);
	}
}
