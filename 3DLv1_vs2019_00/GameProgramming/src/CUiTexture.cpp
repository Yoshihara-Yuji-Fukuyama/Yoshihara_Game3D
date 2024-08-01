#include "CUiTexture.h"
#include "CXPlayer.h"

#define TEX_HP_MAX 0,625,0,90 //HP�ő�

//static�ϐ��̒�`
CTexture CUiTexture::mTextureHpBar;

//static���\�b�h�̒�`
CTexture* CUiTexture::GetTextureHpBar()
{
	return &mTextureHpBar;
}

CUiTexture::CUiTexture()
	:CCharacter()
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

	//HP�̊������v�Z
	float mHpPar = mHp / mMaxHp;

	//����HP
	if (mUiType == EUiType::Hp)
	{
		
	}
}