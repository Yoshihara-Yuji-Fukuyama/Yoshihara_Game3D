#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
	mCharaTag = ECharaTag::EENEMY;
}

//�Փˏ���
void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//�Փ˂��Ă���Ȃ�
	if (m->Collision(m, o) == true)
	{
		//�����̃R���C�_�^�C�v�����Ń^�O����
		//����̃R���C�_�^�C�v�����Ń^�O�����A�e���v���C���[�Ȃ�
		if (m->GetType() == CCollider::EType::ESPHERE && m->GetTag() == CCollider::ETag::EBODY &&
			o->GetType() == CCollider::EType::ESPHERE && o->GetTag() == CCollider::ETag::ESWORD &&
			o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
		{
			//30�t���[�������ă_�E�����A�J��Ԃ��Ȃ�
			ChangeAnimation(11, false, 30);
		}
	}
}

//���R���C�_�̐����ƃ{�[���Ƃ̘A��
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword0.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword1.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword2.SetMatrix(&mpCombinedMatrix[26]);
}
