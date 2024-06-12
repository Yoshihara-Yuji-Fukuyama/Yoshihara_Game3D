#include "CPaladin.h"

#define PLAYER_SPEED 0.1f//�ړ����x
//�ǉ��̃A�j���[�V�����Z�b�g
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
	//����Ɉ����������鐔�l
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

