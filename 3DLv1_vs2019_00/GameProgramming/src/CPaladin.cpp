#include "CPaladin.h"

#define PLAYER_SPEED 0.1f//移動速度
//追加のアニメーションセット
#define TEST "res\\Ch44_nonPBR.x"

CModelX CPaladin::sModel;

CPaladin::CPaladin()
	: mWepon(this, &mMatrix, CVector(-5.0f, 0.0f, -10.0f), &mRotation)
{
	if (sModel.IsLoaded() == false)
	{
		sModel.Load(TEST);
	}
	Init(&sModel);
	//左手に引き金がくる数値
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

void CPaladin::WeponUpdate()
{
	mWepon.Update();
}

void CPaladin::WeponRender()
{
	mWepon.Render();
}

