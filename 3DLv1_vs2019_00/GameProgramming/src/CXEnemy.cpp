#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
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
	mColSphereHead.SetMatrix(&mpCombinedMatrix[11]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[8]);
	//��
	mColSphereSword.SetMatrix(&mpCombinedMatrix[21]);
}
