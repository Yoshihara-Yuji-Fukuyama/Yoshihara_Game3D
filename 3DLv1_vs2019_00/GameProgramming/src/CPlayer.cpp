//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f)//��]���x
#define ROTATION_YV CVector(0.0f,1.0f,0.0f)//��]���x
#define VELOCITY CVector(0.0f,0.0f,0.1f)//�ړ����x

CPlayer* CPlayer::spInstance = nullptr;

CPlayer* CPlayer::Instance()
{
	return spInstance;
}


//CPlayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);//�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.Key(VK_SPACE))
	{
		CBullet* bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->SetPosition(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->SetRotation(mRotation);
		bullet->Update();
		//CApplication::TaskManager()->Add(bullet);
	}         
	//S�L�[���͂ŉ�����
	if (mInput.Key('S'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation - ROTATION_XV;
	}
	//W�L�[���͂ŏ����
	if (mInput.Key('W'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation + ROTATION_XV;
	}
	//A�L�[���͂ŉ�]
	if (mInput.Key('A'))
	{
		//Y���̉�]�l�𑝉�
		mRotation = mRotation + ROTATION_YV;
	}

	//D�L�[���͂ŉ�]
	if (mInput.Key('D'))
	{
		//Y���̉�]�l������
		mRotation = mRotation - ROTATION_YV;
	}

	//��L�[���͂őO�i
	if (mInput.Key(VK_UP))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}

	//�ϊ��s��̍X�V
	CTransform::Update();

	//UI�ݒ�
	CApplication::GetUi()->PosY(mPosition.GetY());
	CApplication::GetUi()->RotX(mRotation.GetX());
	CApplication::GetUi()->RotY(mRotation.GetY());
}

CPlayer::CPlayer()
	: mLine(this, &mMatrix, CVector(0.0f, 0.0f, -14.0f), CVector(0.0f, 0.0f, 17.0f))
	, mLine2(this, &mMatrix, CVector(0.0f, 5.0f, -8.0f), CVector(0.0f, -3.0f, -8.0f))
	, mLine3(this, &mMatrix, CVector(9.0f, 0.0f, -8.0f), CVector(-9.0f, 0.0f, -8.0f))
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->Type())
	{
	case CCollider::EType::ELINE://�����R���C�_
			//����̃R���C�_���O�p�R���C�_�̎�
		if (o->Type() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;//�����p�x�N�g��
		    //�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�̍X�V(mPosition + adjust)
				mPosition = mPosition + adjust;
				//�s��̍X�V
				CTransform::Update();
			}
		}

			break;
	}
}

//�Փˏ���
void CPlayer::Collision()
{
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mLine3, COLLISIONRANGE);
}