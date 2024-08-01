#include "CUiTexture.h"
#include "CXPlayer.h"

#define TEX_HP_MAX 0,625,0,90 //HP最大

//static変数の定義
CTexture CUiTexture::mTextureHpBar;

//staticメソッドの定義
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
	//HPの最大値を設定
	mMaxHp = CXPlayer::GetInstance()->GetHp();
	if (mUiType == EUiType::Hp)
	{
		Texture(GetTextureHpBar(), TEX_HP_MAX);
	}
}

//更新
void CUiTexture::Update()
{
	//現在のHPを設定
	mHp = CXPlayer::GetInstance()->GetHp();

	//HPの割合を計算
	float mHpPar = mHp / mMaxHp;

	//現在HP
	if (mUiType == EUiType::Hp)
	{
		
	}
}