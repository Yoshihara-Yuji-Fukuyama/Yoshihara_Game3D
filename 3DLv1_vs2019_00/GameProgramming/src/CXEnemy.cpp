#include "CXEnemy.h"
#include "CXPlayer.h"

#define GRAVITY_AND_JUMPDEF 0.1f//�d�͂ƃW�����v�̊�l

CModelX CXEnemy::sModel;

CXEnemy::CXEnemy()
	: mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f, CCollider::ETag::EHEAD)//��,���R���C�_
	, mColSphereLeg(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f, CCollider::ETag::ELEG)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
	, mColSphereSearch(this, nullptr, CVector(), 30.0f, CCollider::ETag::ESEARCH)//���G�p�A���R���C�_
{
	//���f����ǂݍ���łȂ��Ȃ�ǂݍ���
	if (sModel.IsLoaded() == false)
	{
		//�G3D���f���t�@�C���ǂݍ���
		sModel.Load(MODEL_PLAYER);//0:�O����
		//�ǉ��A�j���[�V�����ǂݍ���
		sModel.AddAnimationSet(BACKWARD); //1:������
		sModel.AddAnimationSet(L_WALK);   //2:������
		sModel.AddAnimationSet(R_WALK);   //3:�E����
		sModel.AddAnimationSet(AIM_IDLE); //4:�\���ҋ@
		sModel.AddAnimationSet(Fire);     //5:�ˌ�
		sModel.AddAnimationSet(JUMP_UP);  //6:�W�����v
		sModel.SeparateAnimationSet(6, 5, 16, "JumpUp");//7:�W�����v�㏸
		sModel.AddAnimationSet(JUMP_DOWN);//8:�W�����v�~��
		sModel.AddAnimationSet(RUN);//9:����
		sModel.AddAnimationSet(IDLE_RELOAD);//10:�~�܂��ă����[�h
		sModel.AddAnimationSet(WALK_RELOAD);//11:�����Ȃ��烊���[�h
		sModel.AddAnimationSet(HIT);//12:��e
		sModel.AddAnimationSet(WALK_HIT);//13:�����Ȃ����e
		sModel.AddAnimationSet(DEATH);//14:���S
	}
	//���f���̐ݒ�
	Init(&sModel);
	//�^�O�ɓG��ݒ�
	mCharaTag = ECharaTag::EENEMY;
	//�T�C�Y�ݒ�
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//�O��������
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
}
//���W��ݒ�
CXEnemy::CXEnemy(CVector pos)
	: CXEnemy()
{
	mPosition = pos;
}


//�X�V
//�G�̓���
void CXEnemy::Update()
{
	//���S�A�j���[�V�������I����������
	if (GetAnimationIndex() == 14)
	{
		if (IsAnimationFinished())
		{
			mEnabled = false;
		}
	}
	//���S���Ă��Ȃ���΍X�V
	if (IsDead() == false)
	{
		//�L�����N�^�̑O��
		CVector charZ = mMatrixRotate.GetVectorZ();
		//XZ���ʂɂ��Đ��K��
		charZ.SetY(0.0f); charZ = charZ.Normalize();

		//�㏸������Ȃ����n�ʂɐڐG���Ă��Ȃ�����Y���W��-1�ȏ�Ȃ�d�͓K�p
		if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() >= -1.0f)
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
			//�����Ȃ���̃����[�h�Ȃ�
			if (GetAnimationIndex() == 11)
			{
				mPosition = mPosition - charZ * mSpeed;
			}
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

		//�e���؂ꂽ��A�e���[�{�����[�h�A�j���[�V�����Đ�
		if (mWepon.GetAmmo() == 0 && IsRun == false && IsReloading == false)
		{
			//�����Ă���Ȃ瓮���Ȃ���̃����[�h�A�j���[�V����
			if (IsMove == true)
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

		//�v���C���[�������Ă�����
		if (IsFoundPlayer == true)
		{
			//�e������A�����[�h���Ă��Ȃ��Ȃ猂��
			if (mWepon.GetAmmo() > 0 && IsReloading == false)
			{
				mWepon.ShotBullet();
			}
			//�v���C���[�̕��֐i��
			//�v���C���[�ւ̕������v�Z
			CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
			//�v���C���[�Ƃ̋�����10�ȏ�̏ꍇ�߂Â�
			if (moveDirection.Length() > 10.0f && IsReloading == false)
			{
				moveDirection = moveDirection.Normalize();
				mPosition = mPosition - moveDirection * mSpeed;
				ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
				IsMove = true;
			}
			//�v���C���[�Ƃ̋�����10�����̏ꍇ�ҋ@�A�j���[�V�����ɂ���
			else if (IsReloading == false)
			{
				moveDirection = moveDirection.Normalize();
				ChangeAnimation(4, true, 90);
				IsMove = false;
			}
			//�ړ���������������
			ChangeDirection(charZ, moveDirection * -1, 0.6f);
		}
		//�����Ă��Ȃ��Ȃ�
		else
		{
			ChangeAnimation(4, true, 90);
		}
	}
	
	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
	//�J�v�Z���R���C�_�̍X�V
	mColBody.Update();
}

//����̍X�V
void CXEnemy::WeponUpdate()
{
	mWepon.Update();
}
//����̕`��
void CXEnemy::WeponRender()
{
	mWepon.Render();
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
		//�G�̍U���ƏՓ˂�����_���[�W
	case CCollider::EType::ESPHERE:
		//�����̃R���C�_�̃^�O����
		if (m->GetTag() == CCollider::ETag::EHEAD)
		{
			//����̃R���C�_����
			//�^�O�͒e��
			//�e(�e��)�̐e(����)�̐e(�L�����N�^)���v���C���[�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					mHp--;
					//��e�A�j���[�V����
					ChangeAnimation(12, false, 30);
				}
			}
		}
		//�����̃R���C�_�̃^�O����
		else if (m->GetTag() == CCollider::ETag::EBODY)
		{
			//����̃R���C�_����
			//�^�O�͒e��
			//�e(�e��)�̐e(����)�̐e(�L�����N�^)���v���C���[�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					mHp--;
					//��e�A�j���[�V����
					ChangeAnimation(12, false, 30);
				}
			}
		}
		//�����̃R���C�_�̃^�O����
		else if (m->GetTag() == CCollider::ETag::ELEG)
		{
			//����̃R���C�_����
			//�^�O�͒e��
			//�e(�e��)�̐e(����)�̐e(�L�����N�^)���v���C���[�Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					mHp--;
					//��e�A�j���[�V����
					ChangeAnimation(12, false, 30);
				}
			}
		}
		//�����̃R���C�_�̃^�O�����G�p
		else if (m->GetTag() == CCollider::ETag::ESEARCH)
		{
			//����̃R���C�_���v���C���[�̂��̂Ȃ�
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::ELEG &&
				o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//�Փ˂��Ă���Ȃ�
				if (m->Collision(m, o) == true)
				{
					//�v���C���[�������Ă���
					IsFoundPlayer = true;
				}
				//�Փ˂��Ă��Ȃ��Ȃ�
				else
				{
					//�v���C���[�������Ă��Ȃ�
					IsFoundPlayer = false;
				}
			}
		}
		//���肪�O�p�R���C�_�����������G�p�ȊO�̎�
		if (o->GetType() == CCollider::EType::ETRIANGLE && m->GetTag() != CCollider::ETag::ESEARCH)
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

//���R���C�_�̐����ƃ{�[���Ƃ̘A��
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//��
	mColSphereLeg.SetMatrix(&mpCombinedMatrix[1]);
	//�L�������m���d�Ȃ�Ȃ����߂̑̃R���C�_
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//�v���C���[���G�p�R���C�_
	mColSphereSearch.SetMatrix(&mpCombinedMatrix[5]);
	//����Ɉ����������鐔�l
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}
