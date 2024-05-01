#include "CEnemy3.h"
#include "CCollisionManager.h"

#define OBJ "res\\f16.obj"//���f���̃t�@�C��
#define MTL "res\\f16.mtl"//���f���̃}�e���A���t�@�C��
#define HP 3 //�ϋv�l
#define VELOCITY 0.11f//���x

CModel CEnemy3::sModel;//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CEnemy3::CEnemy3()
	:CCharacter3(1)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.4f)
	, mHp(HP)
{
	//���f�����Ȃ��Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
}

//�R���X�g���N�^
//CEnemy(�ʒu,��],�g�k)
CEnemy3::CEnemy3(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CEnemy3()//�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu,��],�g�k��ݒ肷��
	mPosition = position;//�ʒu�̐ݒ�
	mRotation = rotation;//��]�̐ݒ�
	mScale = scale;//�g�k�̐ݒ�
	CTransform::Update();//�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//�X�V����
void CEnemy3::Update()
{
	//HP��0�ȉ��̎����j
	if (mHp <= 0)
	{
		mHp--;
		//15�t���[�����ƂɃG�t�F�N�g
		if (mHp % 15 == 0)
		{
			//�G�t�F�N�g����
			new CEffect(mPosition, 1.0f, 1.0f, "exp.tga", 4, 4, 2);
		}
		//���~������
		mPosition = mPosition - CVector(0.0f, 0.03f, 0.0f);
		CTransform::Update();
		return;
	}
	//�v���C���[�̃|�C���^��0�ȊO�̎�
	CPlayer* player = CPlayer::Instance();
	if (player != nullptr)
	{
		//�v���C���[�܂ł̃x�N�g�������߂�
		CVector vp = player->GetPosition() - mPosition;
		//���x�N�g���Ƃ̓��ς����߂�
		float dx = vp.Dot(mMatrixRotate.GetVectorX());
		//��x�N�g���Ƃ̓��ς����߂�
		float dy = vp.Dot(mMatrixRotate.GetVectorY());
		//�O�x�N�g���Ƃ̓��ς����߂�
		float dz = vp.Dot(mMatrixRotate.GetVectorZ());

		//X���̂��ꂪ2.0�ȉ�
		if (-2.0f < dx && dx < 2.0f)
		{
			//Y���̃Y����2.0�ȉ�
			if (-2.0f < dy && dy < 2.0f)
			{
				//Z���̃Y����+30.0�ȓ�
				if (0.0f < dz && dz <= 30.0f)
				{
					//�e�𔭎˂��܂�
					CBullet* bullet = new CBullet();
					bullet->Set(0.1f, 1.5f);
					bullet->SetPosition(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
					bullet->SetRotation(mRotation);
					bullet->Update();
				}
			}
		}
	}
	//�ڕW�n�_�܂ł̃x�N�g�������߂�
	CVector vp = mPoint - mPosition;
	//�ۑ�
	//���x�N�g���Ƃ̓��ς����߂�
	float dx = vp.Dot(mMatrixRotate.GetVectorX());
	//��x�N�g���Ƃ̓��ς����߂�
	float dy = vp.Dot(mMatrixRotate.GetVectorY());
	const float margin = 0.1f;
	//���E�����։�]
	if (dx > margin)
	{
		//���։�]
		mRotation = mRotation + CVector(0.0f, 1.0f, 0.0f);
	}
	else if (dx < -margin)
	{
		//�ۑ�
		//�E�։�]
		mRotation = mRotation + CVector(0.0f, -1.0f, 0.0f);
	}
	//�㉺�����։�]
	if (dy > margin)
	{
		//��։�]
		mRotation = mRotation + CVector(-1.0f, 0.0f, 0.0f);
	}
	else if (dy < -margin)
	{
		//�ۑ�
		//���։�]
		mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);
	}
	//�@�̑O���ֈړ�����
	mPosition = mPosition + mMatrixRotate.GetVectorZ() * VELOCITY;
	CTransform::Update();//�s��X�V
	//���悻3�b���ƂɖڕW�n�_���X�V
	int r = rand() % 180;//rand()�͐����̗�����Ԃ�
	                     //%180��180�Ŋ������]������߂�
	if (r == 0)
	{
		if (player != nullptr)
		{
			mPoint = player->GetPosition();
		}
		else
		{
			mPoint = mPoint * CMatrix().SetRotateY(45);
		}
	}
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CEnemy3::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::EType::ESPHERE://���R���C�_�̎�
			//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			mHp--;//�q�b�g�|�C���g�̌��Z
			//�G�t�F�N�g����
			new CEffect(o->Parent()->GetPosition(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			//�Փ˂��Ă��鎞�͖����ɂ���
			//mEnabled=false;
		}
		break;
	case CCollider::EType::ETRIANGLE://�O�p�R���C�_�̎�
		CVector adjust;//�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//���j�Œn�ʂɏՓ˂���Ɩ���
			if (mHp <= 0)
			{
				mEnabled = false;
			}
			//�Փ˂��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
		}
		break;
	}
}

void CEnemy3::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}