#include "CBullet.h"
#include "CCollisionManager.h"

#define Bullet_Speed 0.1f

#define AR_OBJ "res\\Guns\\ammo_machinegun.obj"
#define AR_MTL "res\\Guns\\ammo_machinegun.mtl"

CModel CBullet::sModel;//���f���f�[�^�쐬

CBullet::CBullet(CCharacter3* parent)
	:mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.1f, CCollider::ETag::EBULLET)
{
	//���f�����Ȃ��Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(AR_OBJ, AR_MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
	//�e�̐ݒ�
	mpParent = parent;
}

//�傫���̐ݒ�
void CBullet::SetScale(float scale)
{
	//�X�P�[���ݒ�
	mScale = CVector(scale, scale, scale);
}

//�X�V
void CBullet::Update()
{
	//�������Ԃ̔���
	if (mLife-- > 0)
	{
		CTransform::Update();
		//�ʒu�X�V
		mPosition = mPosition + mMoveDirection * Bullet_Speed;
	}
	else
	{
		//�����ɂ���
		mEnabled = false;
	}
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//���肪���G�p�ȊO�̎��R���C�_��m��o���Փ˂��Ă��邩����
	if (CCollider::Collision(m, o) && o->GetTag() != CCollider::ETag::ESEARCH)
	{
		//�Փ˂��Ă���Ƃ��͖����ɂ���
		mEnabled = false;
	}
}
//����������ݒ�
void CBullet::SetDirection()
{
	//TODO:����������ݒ�
}

//�Փˏ���
void CBullet::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}



