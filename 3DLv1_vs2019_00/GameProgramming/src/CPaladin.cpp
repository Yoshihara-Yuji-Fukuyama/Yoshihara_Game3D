#include "CPaladin.h"

#define PALADIN_MODEL_PATH "res\\paladin\\paladin.x"
//�ǉ��̃A�j���[�V�����Z�b�g
#define ANIMATION_ATTACKSP1 "res\\paladin\\SwordAndShieldAttack.x"
#define ANIMATION_RUN "res\\testRun.x"

CModelX CPaladin::sModel;

CPaladin::CPaladin()
{
	if (sModel.IsLoaded() == false)
	{
		sModel.Load(PALADIN_MODEL_PATH);
		//�A�j���[�V�����̒ǉ�
		sModel.AddAnimationSet(ANIMATION_ATTACKSP1);
		sModel.AddAnimationSet(ANIMATION_RUN);
	}
	Init(&sModel);
}
