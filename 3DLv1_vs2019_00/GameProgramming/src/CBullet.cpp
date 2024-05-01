#include "CBullet.h"
#include "CCollisionManager.h"

#define VELOCITY CVector(0.0f,0.0f,1.0f)

CBullet::CBullet()
	:mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.1f)
{}

//���Ɖ��s���̐ݒ�
//Set�i���A���s���j
void CBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_�ݒ�
	mT.Vertex(CVector(w, 0, 0), CVector(0, 0, -d), CVector(-w, 0, 0));
	//�O�p�`�̖@���ݒ�
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CBullet::Update()
{
	//�������Ԃ̔���
	if (mLife-- > 0)
	{
		CTransform::Update();
		//�ʒu�X�V
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	else
	{
		//�����ɂ���
		mEnabled = false;
	}
}

//�`��
void CBullet::Render()
{
	//DIFFUSE���F�ݒ�
	float c[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//�O�p�`�`��
	mT.Render(mMatrix);
	//�R���C�_�̕`��
	//mCollider.Render();
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//�R���C�_��m��o���Փ˂��Ă��邩����
	if (CCollider::Collision(m, o))
	{
		//�Փ˂��Ă���Ƃ��͖����ɂ���
		mEnabled = false;
	}
}

//�Փˏ���
void CBullet::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}



