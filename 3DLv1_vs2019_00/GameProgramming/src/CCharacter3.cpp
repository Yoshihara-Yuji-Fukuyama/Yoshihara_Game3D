#include "CCharacter3.h"
#include "CApplication.h"

//���f���̐ݒ�
void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}
//�e�̃|�C���^���擾
CCharacter3* CCharacter3::GetParent()
{
	return mpParent;
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

//�L�����̕��������������������ɕς���
void CCharacter3::ChangeDirection(CVector charZ, CVector direction, float margin)
{
	//�V��
	float MARGIN = margin;
	//�����̌����ƌ��������������ŊO��
	float cross = charZ.Cross(direction).GetY();
	//�����̌����ƌ��������������œ���
	float dot = charZ.Dot(direction);
	//�O�ς��v���X�͉E��]
	if (cross > MARGIN)
	{
		mRotation.SetY(mRotation.GetY() - 5.0f);
	}
	//�O�ς��}�C�i�X�͍���]
	else if (cross < -MARGIN)
	{
		mRotation.SetY(mRotation.GetY() + 5.0f);
	}
	//�O��̌����������Ƃ����ς�1.0f
	//���������������������Ă����牽�����Ȃ�
	else if (dot < 1.0f - MARGIN)
	{

	}
}