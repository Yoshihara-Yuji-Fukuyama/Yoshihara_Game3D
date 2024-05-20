#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//��]���x
#define PLAYER_SPEED 0.1f//�ړ����x

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//��,���R���C�_
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//��,���R���C�_
{
	//�^�O�Ƀv���C���[��ݒ�
	mCharaTag = ECharaTag::EPLAYER;
	spInstance = this;
}

//�C���X�^���X�̎擾
CXPlayer* CXPlayer::GetInstance()
{
	return spInstance;
}

void CXPlayer::Update()
{
	//�U�����[�V�����̂Ƃ�
	if (GetAnimationIndex() == 3)
	{
		//�A�j���[�V�������I��������
		//�U���I���A�j���[�V�����ɕύX
		if (IsAnimationFinished() == true)
		{		
			ChangeAnimation(4, false, 30);
		}
	}
	//�U���I�����[�V�����̂Ƃ�
	else if (GetAnimationIndex() == 4)
	{
		//�A�j���[�V�������I��������
		//�ҋ@�A�j���[�V�����ɕύX
		if (IsAnimationFinished() == true)
		{	
			ChangeAnimation(0, true, 60);
		}
	}
	//��L�ȊO�̂Ƃ�
	else
	{
		//�ړ������̐ݒ�
		CVector move;
		if (mInput.Key('A'))
		{
			move.SetX(move.GetX() + 1.0f);
		}
		if (mInput.Key('D'))
		{
			move.SetX(move.GetX() - 1.0f);
		}
		if (mInput.Key('W'))
		{
			move.SetZ(move.GetZ() + 1.0f);
		}
		if (mInput.Key('S'))
		{
			move.SetZ(move.GetZ() - 1.0f);
		}
		//�ړ�����
		if (move.Length() > 0.0f)
		{
			move = move.Normalize();
			//�ړ������ֈړ�
			mPosition = mPosition + move * PLAYER_SPEED;
			ChangeAnimation(1, true, 60);
		}
		else
		{
			ChangeAnimation(0, true, 60);
		}
		//���N���b�N�������ꂽ��A�U�����[�V�����ɕύX
		if (mInput.Key(VK_LBUTTON))
		{
			ChangeAnimation(3, false, 30);
		}
	}
	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
	//�J�v�Z���R���C�_�̍X�V
	mColBody.Update();
}

//�Փ˔���
void CXPlayer::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
		//�����̃R���C�_���J�v�Z���̏ꍇ
	case CCollider::EType::ECAPSULE:
		//����̃R���C�_���J�v�Z���Ȃ�
		if (o->GetType() == CCollider::EType::ECAPSULE)
		{
			CVector adjust;//�����p
			//�J�v�Z�����m���Փ˂��Ă���Ȃ�
			if (CCollider::CollisionCapsuleCapseule(m, o, &adjust))
			{
				//�Փ˂��Ȃ��ʒu�ɖ߂�
				mPosition = mPosition + adjust;
				//�s��X�V
				CTransform::Update();
			}
		}
		break;
	}
}

//�����ݒ�
//���R���C�_�̐����ƁA�{�[���Ƃ̘A��
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[12]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[9]);
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
}
