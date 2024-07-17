#include "CXEnemy.h"
#include "CXPlayer.h"
#include "CApplication.h"

#define GRAVITY_AND_JUMPDEF 0.1f//�d�͂ƃW�����v�̊�l

CModelX CXEnemy::sModel;

CXEnemy::CXEnemy()
	: mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f, CCollider::ETag::EHEAD)//��,���R���C�_
	, mColSphereBody(this, nullptr, CVector(0.0f, 0.0f, 0.0f), 0.5f)//��,���R���C�_
	, mColSphereLeg(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f, CCollider::ETag::ELEG)//��,���R���C�_
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//��,�J�v�Z���R���C�_
	, mColSphereSearch(this, nullptr, CVector(), 60.0f, CCollider::ETag::ESEARCH)//���G�p�A���R���C�_
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
	//AI�����ݒ�
	CEnemyAi::Init();
	//�^�O�ɓG��ݒ�
	mCharaTag = ECharaTag::EENEMY;
	//�T�C�Y�ݒ�
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//�O��������
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
	//�n�܂�̎��Ԃ�ۑ�
	start = clock();
	//�b���J�E���^��1��
	mCount = 1;
}
//���W��ݒ�
CXEnemy::CXEnemy(CVector pos)
	: CXEnemy()
{
	mPosition = pos;
}

//���R���C�_�̐����ƃ{�[���Ƃ̘A��
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[7]);
	//��
	mColSphereLeg.SetMatrix(&mpCombinedMatrix[1]);
	//�L�������m���d�Ȃ�Ȃ����߂̑̃R���C�_
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//�v���C���[���G�p�R���C�_
	mColSphereSearch.SetMatrix(&mpCombinedMatrix[5]);
	//����Ɉ����������鐔�l
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

//�G�̐��������̐��������炷
CXEnemy::~CXEnemy()
{
	CApplication::GetInstance()->DeathEnemy();
}


//�X�V
//�G�̓���
void CXEnemy::Update()
{
	//�㏸������Ȃ����n�ʂɐڐG���Ă��Ȃ�����Y���W��-1�ȏ�Ȃ�d�͓K�p
	if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() >= -1.0f)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	//HP��0�ȉ��Ȃ玀�S
	if (mHp <= 0)
	{
		ChangeState(CEnemyAi::EAiState::EDIE);
	}

	switch (mAiState)
	{
	case CEnemyAi::EAiState::EWAIT://�ҋ@
		Wait();
		break;
	case CEnemyAi::EAiState::EWANDERING://�p�j
		Wandering();
		break;
	case CEnemyAi::EAiState::ECHASE://�ǐ�
		Chase();
		break;
	case CEnemyAi::EAiState::EATTACK://�U��
		Attack();
		break;
	case CEnemyAi::EAiState::EMOVEATTACK://�ړ����Ȃ���U��
		MoveAttack();
		break;
	case CEnemyAi::EAiState::ERELOAD://�����[�h
		Reload();
		break;
	case CEnemyAi::EAiState::EESCAPE://���S
		Escape();
		break;
	case CEnemyAi::EAiState::EDAMAGE://��e
		Damage();
		break;
	case CEnemyAi::EAiState::EDIE://���S
		Die();
		break;
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

bool CXEnemy::IsTime(int lag)
{
	clock_t end = clock();//�o�ߎ���
	double sec = (double)(end - start) / CLOCKS_PER_SEC;//�b���ϊ�
	if (sec >= mCount)
	{
		mCount = mCount + lag;
		return true;
	}
	return false;
}

void CXEnemy::Wait()//�ҋ@
{
	//�ҋ@�A�j���[�V����
	ChangeAnimation(4, true, 90);
	if (IsFoundPlayer == true)
	{
		ChangeState(CEnemyAi::EAiState::ECHASE);
	}
}

void CXEnemy::Wandering()//�p�j
{
	//�v���C���[���������Ȃ�A�ǐ�
	if (IsFoundPlayer == true)
	{
		ChangeState(CEnemyAi::EAiState::ECHASE);
	}
}

void CXEnemy::Chase()//�ǐ�
{
	//�L�����N�^�̑O��
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ���ʂɂ��Đ��K��
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//�v���C���[�̕��֐i��
	//�v���C���[�ւ̕������v�Z
	CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
	//�v���C���[�Ƃ̋�����30�ȏ�̏ꍇ�߂Â�
	if (moveDirection.Length() > 30.0f)
	{
		moveDirection = moveDirection.Normalize();
		mPosition = mPosition - moveDirection * mSpeed;
		ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
	}
	//������߂���΍U��
	else
	{
		ChangeState(CEnemyAi::EAiState::EATTACK);
	}
	//�ړ���������������
	ChangeDirection(charZ, moveDirection * -1, 0.06f);

	if (IsFoundPlayer == false)
	{
		ChangeState(CEnemyAi::EAiState::EWANDERING);
	}
}

void CXEnemy::Attack()//�U��
{
	//�L�����N�^�̑O��
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ���ʂɂ��Đ��K��
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//�v���C���[�̕��֐i��
	//�v���C���[�ւ̕������v�Z
	CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
	//�v���C���[�̕�������������
	ChangeDirection(charZ, moveDirection * -1, 0.06f);
	//�ˌ�
	mWepon.ShotBullet();
	//�c�e����0�Ȃ烊���[�h
	if (mWepon.GetAmmo() == 0)
	{
		ChangeState(CEnemyAi::EAiState::ERELOAD);
	}
}

void CXEnemy::MoveAttack()
{
	//�ˌ�
	mWepon.ShotBullet();
	//�c�e����0�Ȃ烊���[�h
	if (mWepon.GetAmmo() == 0)
	{
		ChangeState(CEnemyAi::EAiState::ERELOAD);
	}
}

void CXEnemy::Reload()
{
	//�e���؂ꂽ��A�e���[�{�����[�h�A�j���[�V�����Đ�
	if (mWepon.GetAmmo() == 0)
	{
		//�����[�h���[�V����
		ChangeAnimation(10, false, 90);
		mWepon.Reload();
	}
	//�����[�h�A�j���[�V�����̎�
	if (GetAnimationIndex() == 10)
	{
		//�A�j���[�V�������I��������
		//�ҋ@�X�e�[�g�ɂ���
		if (IsAnimationFinished() == true)
		{
			ChangeState(CEnemyAi::EAiState::EWAIT);
		}
	}
}

void CXEnemy::Escape()
{
	//�L�����N�^�̑O��
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ���ʂɂ��Đ��K��
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//�v���C���[�Ƌt�̕��֐i��
	//�v���C���[�Ƌt�ւ̕������v�Z
	CVector moveDirection = CXPlayer::GetInstance()->GetPosition() - mPosition;
	//���K��
	moveDirection = moveDirection.Normalize();
	mPosition = mPosition - moveDirection * mSpeed;
	ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
	//�ړ���������������
	ChangeDirection(charZ, moveDirection * -1, 0.6f);

	//�v���C���[�������Ă��Ȃ���Αҋ@�Ɉڍs
	if (IsFoundPlayer == false)
	{
		ChangeState(CEnemyAi::EAiState::EWAIT);
	}
}

void CXEnemy::Damage()
{
	//��e�A�j���[�V�����̎�
	if (GetAnimationIndex() == 12)
	{
		//�A�j���[�V�������I��������
		//�ҋ@�X�e�[�g�ɂ���
		if (IsAnimationFinished() == true)
		{
			ChangeState(CEnemyAi::EAiState::EWAIT);
		}
	}
}

void CXEnemy::Die()
{
	ChangeAnimation(14, false, 90);
	//���S�A�j���[�V�������I����������
	if (GetAnimationIndex() == 14)
	{
		if (IsAnimationFinished())
		{
			mEnabled = false;
		}
	}
}
