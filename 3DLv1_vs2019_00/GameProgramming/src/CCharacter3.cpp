#include "CCharacter3.h"
#include "CApplication.h"

void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}

//�`�揈��
void CCharacter3::Render()
{
	mpModel->Render(mMatrix);
}

CCharacter3::~CCharacter3()
{
	//�^�X�N���X�g����폜
	CTaskManager::GetInstance()->Remove(this);
}

CCharacter3::ECharaTag CCharacter3::GetCharaTag()
{
	return mCharaTag;
}

CCharacter3::CCharacter3()
	:mpModel(nullptr)
{
	//�^�X�N���X�g�ɒǉ�
	CTaskManager::GetInstance()->Add(this);
}

CCharacter3::CCharacter3(int priority)
	:mpModel(nullptr)
{
	mPriority = priority;
	CTaskManager::GetInstance()->Add(this);
}