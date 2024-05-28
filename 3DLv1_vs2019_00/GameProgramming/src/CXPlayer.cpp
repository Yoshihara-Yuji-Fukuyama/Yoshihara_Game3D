#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//��]���x
#define PLAYER_SPEED 0.1f//�ړ����x

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	/*/ : mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//��,���R���C�_
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//��,���R���C�_
	*/
	
{
	//�^�O�Ƀv���C���[��ݒ�
	mCharaTag = ECharaTag::EPLAYER;
	spInstance = this;
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
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
		//�J�����̑O��
		CVector cameraZ = CActionCamera::GetInstance()->GetVectorZ();
		//�J�����̍�����
		CVector cameraX = CActionCamera::GetInstance()->GetVectorX();
		//�L�����N�^�̑O��
		CVector charZ = mMatrixRotate.GetVectorZ();
		//XZ���ʂɂ��Đ��K��
		cameraZ.SetY(0.0f); cameraZ = cameraZ.Normalize();
		cameraX.SetY(0.0f); cameraX = cameraX.Normalize();
		charZ.SetY(0.0f); charZ = charZ.Normalize();
		//�ړ������̐ݒ�
		CVector move;
		if (mInput.Key('A'))
		{
			move = move + cameraX;
		}
		if (mInput.Key('D'))
		{
			move = move - cameraX;
		}
		if (mInput.Key('W'))
		{
			move = move + cameraZ;
		}
		if (mInput.Key('S'))
		{
			move = move - cameraZ;
		}
		//�ړ�����
		if (move.Length() > 0.0f)
		{
			//�V��
			const float MARGIN = 0.06f;
			//���K��
			move = move.Normalize();
			//�����̌����ƌ��������������ŊO��
			float cross = charZ.Cross(move).GetY();
			//�����̌����ƌ��������������œ���
			float dot = charZ.Dot(move);
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
			else if (dot < 1.0f - MARGIN)
			{
				mRotation.SetY(mRotation.GetY() + 5.0f);
			}
			//�ړ������ֈړ�
			mPosition = mPosition + move * PLAYER_SPEED;
			ChangeAnimation(0, true, 90);
		}
		else
		{
			ChangeAnimation(2, true, 90);
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
	//mColBody.Update();
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
	/*
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[12]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[9]);
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);*/
}
