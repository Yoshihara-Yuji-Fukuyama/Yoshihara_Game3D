#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

//mType�̒l��Ԃ�
CCollider::EType CCollider::GetType()
{
	return mType;
}

//mTag�̒l��Ԃ�
CCollider::ETag CCollider::GetTag()
{
	return mTag;
}

CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mRadius(0)
{
	//�R���W�����}�l�[�W���ɒǉ�
	CCollisionManager::Instance()->Add(this);
}

CCollider::~CCollider()
{
	//�R���W�������X�g����폜
	CCollisionManager::Instance()->Remove(this);
}

CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius, ETag tag)
	: CCollider()
{
	//�^�O�̐ݒ�
	mTag = tag;
	//�e����
	mpParent = parent;
	//�e�s��ݒ�
	mpMatrix = matrix;
	//CTransform�ݒ�
	mPosition = position;//�ʒu
	//���a�ݒ�
	mRadius = radius;
	//�R���W�����}�l�[�W���ɒǉ�
	//CCollisionManager::Instance()->Add(this);
}

void CCollider::Render()
{
	glPushMatrix();
	//�R���C�_�̒��S���W���v�Z
	//�����̍��W�~�e�̕ϊ��s��
	CVector pos = mPosition * *mpMatrix;
	//���S���W�ֈړ�
	glMultMatrixf(CMatrix().SetTranlate(pos.GetX(), pos.GetY(), pos.GetZ()).GetM());
	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//���`��
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

void CCollider::ChangePriority()
{
	//�����̍��W�~�e�̕ϊ��s��������ă��[���h���W�����߂�
	CVector pos = mPosition * *mpMatrix;
	//�x�N�g���̒������D��x
	CCollider::ChangePriority(pos.Length());
}

void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::Instance()->Remove(this);//��U�폜
	CCollisionManager::Instance()->Add(this);//�ǉ�
}

//�R���C�_1��2�̏Փ˔���
bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//�e�R���C�_�̒��S���W�����߂�
	//���_�~�R���C�_�̕ϊ��s��~�e�̕ϊ��s��
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//���S���璆�S�ւ̃x�N�g�������߂�
	mpos = mpos - opos;
	//���S�̋��������a�̍��v��菬�����ƏՓ�
	if (m->mRadius + o->mRadius > mpos.Length())
	{
		//�Փ˂��Ă���
		return true;
	}
	//�Փ˂��Ă��Ȃ�
	return false;
}
//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//���R���C�_�����[���h���W�ō쐬
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(nullptr, nullptr, sv, ev);
	//�O�p�R���C�_�Ɛ��R���C�_�̏Փˏ���
	return CollisionTriangleLine(t, &line, a);
}
//�O�p�R���C�_�Ɛ����R���C�_�̏Փ˔���
bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a)
{
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = sv - v[0];
	//�O�p�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = ev - v[0];
	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f)
	{
		//�Փ˂��ĂȂ�(�����s�v)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���
	
	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	//���_1���_2�x�N�g���ƒ��_1��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_2���_3�x�N�g���ƒ��_2��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_3���_1�x�N�g���ƒ��_3��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����l�v�Z(�Փ˂��Ȃ��ʒu�܂Ŗ߂�)
	if (dots < 0.0f)
	{
		//�n�_������
		*a = normal * -dots;
	}
	else
	{
		//�I�_������
		*a = normal * -dote;
	}
	return true;
}

//�J�v�Z���R���C�_�ƃJ�v�Z���R���C�_�̏Փ˔���
bool CCollider::CollisionCapsuleCapseule(CCollider* m, CCollider* o, CVector* adjust)
{
	CVector mp1, mp2;
	float radius = m->mRadius + o->mRadius;

	*adjust = CVector();
	if (adjust->CalcSegmentSegmentDist(m->GetV(0), m->GetV(1), o->GetV(0), o->GetV(1), &mp1, &mp2) < radius)
	{
		*adjust = mp1 - mp2;
		float len = radius - adjust->Length();
		*adjust = adjust->Normalize() * len;
		return true;
	}
	return false;
}


//�z��mV[i]�̗v�f��Ԃ�
const CVector& CCollider::GetV(int i)
{
	return mV[i];
}


//�e�|�C���^�̎擾
CCharacter3* CCollider::GetParent()
{
	return mpParent;
}

//�e�s��̐ݒ�
void CCollider::SetMatrix(CMatrix* m)
{
	mpMatrix = m;
}
