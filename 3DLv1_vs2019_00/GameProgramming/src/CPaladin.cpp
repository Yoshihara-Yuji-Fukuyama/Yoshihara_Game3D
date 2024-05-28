#include "CPaladin.h"

#define PLAYER_SPEED 0.1f//移動速度
//追加のアニメーションセット
#define TEST "res\\Ch44_nonPBR.x"

CModelX CPaladin::sModel;

CPaladin::CPaladin()
{
	if (sModel.IsLoaded() == false)
	{
		sModel.Load(TEST);
	}
	Init(&sModel);
}

