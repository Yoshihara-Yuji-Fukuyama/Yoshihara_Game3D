#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)//��,���R���C�_
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 1.0f, 0.0f), 1.0f)//��,�J�v�Z���R���C�_
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//��,�J�v�Z���R���C�_
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//��,�J�v�Z���R���C�_
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//��,�J�v�Z���R���C�_
{
	//�^�O�Ƀv���C���[��ݒ�
	mCharaTag = ECharaTag::EENEMY;
}


//�X�V
void CXEnemy::Update()
{
	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
	//�J�v�Z���R���C�_�̍X�V
	mColBody.Update();
}


//�Փˏ���
void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
		//�����̃R���C�_���J�v�Z���̏ꍇ
		//�Փ˂�����d�Ȃ�Ȃ��ʒu�ɖ߂�
	case CCollider::EType::ECAPSULE:
		//����̃R���C�_���J�v�Z���Ȃ�
		if (o->GetType() == CCollider::EType::ECAPSULE)
		{
			CVector adjust;//�����p
			//�J�v�Z�����m���Փ˂��Ă���Ȃ�
			if (CCollider::CollisionCapsuleCapseule(m, o, &adjust))
			{
				//�d�Ȃ�Ȃ��ʒu�ɖ߂�
				mPosition = mPosition + adjust;
				//�s��X�V
				CTransform::Update();
			}
		}
		break;

		//�����̃R���C�_�����̏ꍇ
		//�G�̍U���ƏՓ˂�����_�E������
	case CCollider::EType::ESPHERE:
		//�����̃R���C�_�̃^�O����
		if (m->GetTag() == CCollider::ETag::EBODY)
		{
			//����̃R���C�_����
			//�^�O�͌�
			//�e���v���C���[�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::ESWORD &&
				o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					//30�t���[�������ă_�E�����A�J��Ԃ��Ȃ�
					ChangeAnimation(11, false, 30);
				}

			}
		}
		break;
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
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword0.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword1.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword2.SetMatrix(&mpCombinedMatrix[26]);
}
