#include "CPaladin.h"

#define PALADIN_MODEL_PATH "res\\paladin\\paladin.x"
//追加のアニメーションセット
#define ANIMATION_ATTACKSP1 "res\\paladin\\SwordAndShieldAttack.x"
#define ANIMATION_RUN "res\\testRun.x"

CModelX CPaladin::sModel;

CPaladin::CPaladin()
{
	if (sModel.IsLoaded() == false)
	{
		sModel.Load(PALADIN_MODEL_PATH);
		//アニメーションの追加
		sModel.AddAnimationSet(ANIMATION_ATTACKSP1);
		sModel.AddAnimationSet(ANIMATION_RUN);
	}
	Init(&sModel);
}
