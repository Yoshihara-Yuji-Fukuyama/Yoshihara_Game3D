#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//��]���x
#define PLAYER_SPEED 0.1f//�ړ����x
#define GRAVITY_AND_JUMPDEF 0.1f//�d�͂ƃW�����v�̊�l

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: IsLeftTurn(false)
	, IsRightTurn(false)
	, IsGround(false)
	, IsJump(false)
	, mJumpPower(1.0f)
	, mWepon(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//��,���R���C�_
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//��,���R���C�_
	//, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//��,���R���C�_
{
	//�^�O�Ƀv���C���[��ݒ�
	mCharaTag = ECharaTag::EPLAYER;
	//�C���X�^���X�ɐݒ�
	spInstance = this;
	//�T�C�Y�ݒ�
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//�O��������
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
}

//�C���X�^���X�̎擾
CXPlayer* CXPlayer::GetInstance()
{
	return spInstance;
}

void CXPlayer::Update()
{

	//�㏸������Ȃ�����Y���W��0�ȏ�Ȃ�d�͓K�p
	if (GetAnimationIndex() != 7 && mPosition.GetY() > 0)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	//�W�����v�㏸�A�j���[�V�����̎�
	if (GetAnimationIndex() == 7)
	{
		mPosition.SetY(mPosition.GetY() + GRAVITY_AND_JUMPDEF);
		//�A�j���[�V�������I��������
		//�~���A�j���[�V�����ɂ���
		if (IsAnimationFinished() == true)
		{
			ChangeAnimation(8, false, 90 * mJumpPower);
		}
	}
	//�W�����v�~���A�j���[�V�����̎�
	else if (GetAnimationIndex() == 8)
	{
		//�A�j���[�V�������I��������
		//�ҋ@�A�j���[�V�����ɂ���
		if (IsAnimationFinished() == true)
		{
			ChangeAnimation(1, true, 90);
			IsJump = false;
		}
	}
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
	//�ǂ̕����Ɉړ����Ă��邩
	bool moveF = false;//�O
	bool moveB = false;//���
	bool moveL = false;//��
	bool moveR = false;//�E
	//�O�i
	if (mInput.Key('W'))
	{
		move = move + cameraZ;
		moveF = true;
	}
	//���
	else if (mInput.Key('S'))
	{
		move = move - cameraZ;
		moveB = true;
	}
	//���ړ�
	if (mInput.Key('A'))
	{
		move = move + cameraX;
		moveL = true;
	}
	//�E�ړ�
	else if (mInput.Key('D'))
	{
		move = move - cameraX;
		moveR = true;
	}
	//�W�����v
	if (mInput.Key(VK_SPACE) && IsJump == false)
	{
		ChangeAnimation(7, false, 45 * mJumpPower);
		IsJump = true;
	}
	//���N���b�N�������ꂽ��A�e�ۂ��΂�
	if (mInput.Key(VK_LBUTTON))
	{

	}
	//R�L�[�������ꂽ��A�e���[�{�����[�h�A�j���[�V�����Đ�
	if (mInput.Key('R'))
	{

	}
	//�ړ�����
	if (move.Length() > 0.0f)
	{
		//���K��
		move = move.Normalize();
		//�ړ������ֈړ�
		mPosition = mPosition + move * PLAYER_SPEED;
		//�W�����v���łȂ��Ȃ�
		//���ʈړ�
		//�A�j���[�V�����ύX�ƕ����ύX
		if (moveF == true && IsJump == false)
		{

			//�O�ړ�
			ChangeAnimation(0, true, 90);
			//�ړ���������������
			ChangeDirection(charZ, move);
		}
		//�W�����v���łȂ��Ȃ�
		//���ړ�
		//�A�j���[�V�����ύX�ƕ����ύX
		else if (moveB == true && IsJump == false)
		{
			//���ړ�
			ChangeAnimation(1, true, 90);
			//�ړ������̋t����������
			ChangeDirection(charZ, move * -1);
		}
		//�W�����v���łȂ��Ȃ�
		//���ړ�
		//�A�j���[�V�����ύX�ƕ����ύX
		else if ((moveL == true || moveR == true) && IsJump == false)
		{
			//���ړ��Ȃ�
			if (moveL == true)
			{
				//�������A�j���[�V�����ɕύX
				ChangeAnimation(2, true, 90);
			}
			//�E�ړ��Ȃ�
			else if (moveR == true && IsJump == false)
			{
				//�E�����A�j���[�V�����ɕύX
				ChangeAnimation(3, true, 90);
			}
			//���ړ����͐��ʂ���������
			ChangeDirection(charZ, cameraZ);
		}
	}
	//�ړ����W�����v�����Ă��Ȃ��ꍇ�A�ҋ@�A�j���[�V�����ɐ؂�ւ�
	//���ʂ�����
	else if (IsJump == false)
	{
		//�ҋ@�A�j���[�V�����ɕύX
		ChangeAnimation(4, true, 90);
		//���ʂ���������
		ChangeDirection(charZ, cameraZ);
	}
	//�W�����v���͐��ʂ���������
	if (IsJump == true)
	{
		ChangeDirection(charZ, cameraZ);
	}
	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
	//�J�v�Z���R���C�_�̍X�V
	//mColBody.Update();
}

//����̍X�V
void CXPlayer::WeponUpdate()
{
	mWepon.Update();
}

//����̕`��
void CXPlayer::WeponRender()
{
	mWepon.Render();
}

//�}�E�X�ړ���Y����]
void CXPlayer::Turn(float turnHorizontal)
{
	//��]�ʂ̕�������]����
	mRotation = mRotation - CVector(0.0f, turnHorizontal, 0.0f);
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
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[5]);
	//mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//��
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
	//TODO:����Ɉ����������鐔�l��T��
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

