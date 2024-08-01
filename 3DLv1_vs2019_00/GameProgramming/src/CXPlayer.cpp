#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//��]���x
#define GRAVITY_AND_JUMPDEF 0.1f//�d�͂ƃW�����v�̊�l
#define MAX_HP 50//�ő�HP

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f, CCollider::ETag::EHEAD)//��,���R���C�_
	, mColSphereLeg0(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f,CCollider::ETag::ELEG)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
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
	//���S�A�j���[�V�������I����������
	if (GetAnimationIndex() == 14)
	{
		if (IsAnimationFinished())
		{
			
		}
	}
	//���S���Ă��Ȃ���΍X�V
	if (IsDead() == false)
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

		//�㏸������Ȃ����n�ʂɐڐG���Ă��Ȃ�����Y���W��0���傫���Ȃ�d�͓K�p
		if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() > -1.0f)
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
				ChangeAnimation(4, true, 90);
				IsJump = false;
			}
		}
		//�����[�h�A�j���[�V�����̎�
		if (GetAnimationIndex() == 10 || GetAnimationIndex() == 11)
		{
			//�A�j���[�V�������I��������
			//�ҋ@�A�j���[�V�����ɂ���
			if (IsAnimationFinished() == true)
			{
				ChangeAnimation(4, true, 90);
				IsReloading = false;
				IsWalkReload = false;
				IsWaitReload = false;
			}
		}
		//��e�A�j���[�V�����̎�
		if (GetAnimationIndex() == 12)
		{
			if (IsAnimationFinished() == true)
			{
				ChangeAnimation(4, true, 90);

				IsRun = false;
				IsJump = false;
				IsReloading = false;
				IsWalkReload = false;
				IsWaitReload = false;
			}
		}
		//���[�����O�̎�
		if (GetAnimationIndex() == 15)
		{
			//�ړ������ֈړ�
			mPosition = mPosition + mMoveSave * mSpeed;
			//�ړ���������������
			ChangeDirection(charZ, mMoveSave);
			if (IsAnimationFinished() == true)
			{
				ChangeAnimation(4, true, 90);
				mSpeed = mSpeed / 1.5f;
				IsRoll = false;
			}
		}

		//�ړ������̐ݒ�
		CVector move;
		bool isMove = false;//�����Ă��邩
		bool isMoveLR = false;//���E�ړ������Ă��邩
		bool isMoveB = false;//���ړ������Ă��邩
		bool isAim = false;//�\���Ă��邩
		//�O�i
		if (mInput.Key('W'))
		{
			move = move + cameraZ;
			isMove = true;
		}
		//���
		if (mInput.Key('S'))
		{
			move = move - cameraZ;
			isMove = true;
			isMoveB = true;
		}

		//���ړ�
		if (mInput.Key('A'))
		{
			move = move + cameraX;
			isMove = true;
			isMoveLR = true;
		}
		//�E�ړ�
		if (mInput.Key('D'))
		{
			move = move - cameraX;
			isMove = true;
			isMoveLR = true;
		}
		//���[�����O
		if (mInput.Key(VK_SPACE) && isMoveLR == true && IsRoll == false && IsJump == false && IsRun == false)
		{
			ChangeAnimation(15, false, 90);
			mSpeed = mSpeed * 1.5f;
			mMoveSave = move;
			IsRoll = true;
		}
		//�W�����v
		else if (mInput.Key(VK_SPACE) && IsJump == false && IsReloading == false && IsRoll == false)
		{
			ChangeAnimation(7, false, 45 * mJumpPower);
			IsJump = true;
		}
		//TODO:�����R���C�_���o��
		//���e�_�����߂Ă���
		//���N���b�N�������ꂽ��A�e�ۂ��΂�
		if (mInput.Key(VK_LBUTTON) && IsRun == false && IsReloading == false)
		{
			//�e�̐���
			mWepon.ShotBullet();
			
		}
		//�E�N���b�N�������ꂽ��A�\����
		if (mInput.Key(VK_RBUTTON) && IsRun == false && IsReloading == false)
		{
			ChangeDirection(charZ, cameraZ);
			isAim = true;
		}
		//R�L�[�������ꂽ��A�e���[�{�����[�h�A�j���[�V�����Đ�
		if (mInput.Key('R') && IsRun == false && IsReloading == false)
		{
			//�����Ă���Ȃ瓮���Ȃ���̃����[�h�A�j���[�V����
			if (isMove == true)
			{
				ChangeAnimation(11, false, 210);
				IsWalkReload = true;
			}
			//�����Ă��Ȃ��Ȃ��~���������[�h�A�j���[�V����
			else
			{
				ChangeAnimation(10, false, 90);
				IsWaitReload = true;
			}
			//�����[�h
			mWepon.Reload();
			IsReloading = true;
		}
		//�ړ�����Shift�L�[�������ꂽ��A�ړ����x�㏸������
		if (mInput.Key(VK_SHIFT) && isMove == true && IsJump == false && IsReloading == false && IsRoll == false)		{
			if (IsRun == false)
			{
				mSpeed = mSpeed * 2.0f;
			}
			IsRun = true;
			mWepon.SetRun(IsRun);
		}
		//������Ă��Ȃ��Ȃ�A���̑��x�ɖ߂�
		else
		{
			if (IsRun == true)
			{
				mSpeed = mSpeed / 2.0f;
			}
			IsRun = false;
			mWepon.SetRun(IsRun);
		}

		//�ړ�����A�~�܂��ă����[�h���͓����Ȃ�
		if (move.Length() > 0.0f && IsWaitReload == false && IsRoll == false)
		{
			//���K��
			move = move.Normalize();
			if (IsWaitReload == false)
			{
				//�ړ������ֈړ�
				mPosition = mPosition + move * mSpeed;
			}
			//���̓��������Ă��Ȃ���΃A�j���[�V�������ړ��ɕς���
			if (IsRun == false && IsJump == false && IsReloading == false && IsRoll == false)
			{
				//�O�ړ�
				ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
			}
			else if (IsRun == true)
			{
				//����ړ�
				ChangeAnimation(9, true, 90 * (0.7f - (mSpeed * 0.1f)));
			}
			//�\���Ă��Ȃ��Ȃ�
			if (isAim == false)
			{
				//�ړ���������������
				ChangeDirection(charZ, move);
			}
		}
		//�ړ����W�����v�������[�h�����Ă��Ȃ��ꍇ�A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		//���ʂ�����
		else if (IsJump == false && IsReloading == false && IsRoll == false)
		{
			//�ҋ@�A�j���[�V�����ɕύX
			ChangeAnimation(4, true, 90);
		}
	}
	
	
	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
	//�J�v�Z���R���C�_�̍X�V
	mColBody.Update();
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
		//����̃R���C�_����
		//�^�O�͒e��
		//�e(�e��)�̐e(����)�̐e(�L�����N�^)���G�Ȃ�
		else if(o->GetType() == CCollider::EType::ESPHERE &&
			o->GetTag() == CCollider::ETag::EBULLET &&
			o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
		{
			//�Փ˂��Ă���Ȃ�
			if (m->CollisionCapsuleSphere(m, o) == true)
			{
				if (IsDead() == false && IsRoll == false)
				{
					mHp--;
					//��e�A�j���[�V����
					ChangeAnimation(12, false, 30);
				}
			}
		}
		break;

		//�����̃R���C�_�����̏ꍇ
		//�G�̍U���ƏՓ˂�����_���[�W
		//�n�ʂƂԂ���ƏՓ˂��Ȃ��ʒu�܂Ŗ߂�
	case CCollider::EType::ESPHERE:
		//�����̃R���C�_�̃^�O����
		if (m->GetTag() == CCollider::ETag::EHEAD)
		{
			//����̃R���C�_����
			//�^�O�͒e��
			//�e(�e��)�̐e(����)�̐e(�L�����N�^)���G�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					if (IsDead() == false && IsRoll == false)
					{
						mHp--;
						//��e�A�j���[�V����
						ChangeAnimation(12, false, 30);
					}
				}
			}
		}
		//�����̃R���C�_�̃^�O����
		else if (m->GetTag() == CCollider::ETag::ELEG)
		{
			//����̃R���C�_����
			//�^�O�͒e��
			//�e(�e��)�̐e(����)�̐e(�L�����N�^)���G�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					if (IsDead() == false && IsRoll == false)
					{
						mHp--;
						//��e�A�j���[�V����
						ChangeAnimation(12, false, 30);
					}
				}
			}
		}
		//���肪�O�p�R���C�_�̂Ƃ�
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;//�����l
			//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
				mPosition = mPosition + adjust;
				IsGround = true;
			}
			else
			{
				IsGround = false;
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
	mColSphereLeg0.SetMatrix(&mpCombinedMatrix[1]);
	//�L�������m���d�Ȃ�Ȃ����߂̑̃R���C�_ 
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//����Ɉ����������鐔�l
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

//TODO
//�񕜏���
void CXPlayer::Heal()
{
	//�ő�l���HP���Ⴂ�Ȃ�2����HP���񕜂���
	if (mHp < MAX_HP)
	{
		mHp += MAX_HP / 5;
	}
	//�ő�l���񕜂��Ă�����ő�l�ɂ���
	if (mHp > MAX_HP)
	{
		mHp = MAX_HP;
	}
}


