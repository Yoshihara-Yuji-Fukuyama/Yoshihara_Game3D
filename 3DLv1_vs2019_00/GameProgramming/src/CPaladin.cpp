#include "CPaladin.h"

#define PLAYER_SPEED 0.1f//�ړ����x
//�ǉ��̃A�j���[�V�����Z�b�g
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

