#include "CUiTexture.h"
#include "CXPlayer.h"


//static変数の定義
CTexture CUiTexture::mTextureHpBar;

//staticメソッドの定義
CTexture* CUiTexture::GetTextureHpBar()
{
	return &mTextureHpBar;
}

CUiTexture::CUiTexture()
{
}

CUiTexture::CUiTexture(float maxHp, float w, float h, EUiType uiType,
	int left, int right, int bottom, int top)
	: CUiTexture()
{
	SetW(w);
	SetH(h);

	mUiType = uiType;
	//HPの最大値を設定
	mMaxHp = maxHp;
	if (mUiType == EUiType::Hp)
	{
		//テクスチャ設定
		Texture(GetTextureHpBar(), TEX_HP_MAX);
		//HPバーの座標を設定
		Set(HP_POS, GetW(), GetH());
	}
}

//更新
void CUiTexture::Update()
{
	HpBarUpdate();//HP表示を更新
}

//HP表示を更新
void CUiTexture::HpBarUpdate()
{
	//HPの割合を計算
	float mHpPar = mHp / mMaxHp;

	if (mHpPar >= 1.0f)//0割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_MAX);
	}
	else if (mHpPar >= 0.9f)//1割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_1);
	}
	else if (mHpPar >= 0.8f)//2割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_2);
	}
	else if (mHpPar >= 0.7f)//3割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_3);
	}
	else if (mHpPar >= 0.6f)//4割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_4);
	}
	else if (mHpPar >= 0.5f)//5割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_5);
	}
	else if (mHpPar >= 0.4f)//6割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_6);
	}
	else if (mHpPar >= 0.3f)//7割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_7);
	}
	else if (mHpPar >= 0.2f)//8割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_8);
	}
	else if (mHpPar >= 0.1f)//9割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_9);
	}
	else //10割減少
	{
		Texture(GetTextureHpBar(), TEX_HP_10);
	}
}

void CUiTexture::SetHp(float hp)
{
	//現在のHPを設定
	mHp = hp;
}
