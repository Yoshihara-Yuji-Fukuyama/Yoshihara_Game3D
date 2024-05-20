#include "CVector.h"
#include <math.h>

CVector::CVector()
	:mX(0.0f)
	, mY(0.0f)
	, mZ(0.0f)
{}

//�R���X�g���N�^
CVector::CVector(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{}

//���K��
CVector CVector::Normalize() const
{
	//�x�N�g���̑傫���Ŋ������x�N�g����Ԃ��i����1�̃x�N�g���j
	return *this * (1.0f / Length());
}

//����
float CVector::Dot(const CVector& v)const
{
	return mX * v.mX + mY * v.mY + mZ * v.mZ;
}

//�O��
CVector CVector::Cross(const CVector& v)const
{
	return CVector(mY * v.mZ - mZ * v.mY, mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX);
}

//�x�N�g���̒�����Ԃ�
float CVector::Length() const
{
	//sqrt�֐��ŕ�������Ԃ�
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

//�_�Ɛ�(�n�_�A�I�_��ʂ钼��)�̍ŒZ���������߂�
float CVector::CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t)
{
	*t = 0.0f;//�����̏�����
	CVector v = e - s;//�n�_����I�_�ւ̃x�N�g�������߂�
	float dvv = v.Dot(v);//�x�N�g���̒�����2������߂�
	if (dvv > 0.0f)
	{
		*t = v.Dot(p - s) / dvv;//����̐����ƂȂ�_�̊��������߂�
		//��̎��̐���
		//dot(v , p - sp)��|v||p - sp|cos��
		//dvv��|v|��2��
		//��̌v�Z�ŁAt�� |p - sp|cos�� / |v|�ƂȂ�
		//�܂�t�́udot�œ��e����������v�̒����v�Ƃ��������ɂȂ�
	}
	*mp = s + v * *t;//����̐����ƂȂ�_�����߂�
	return (p - *mp).Length();//�����̒�����Ԃ�
}

//2���Ԃ̍ŒZ������Ԃ�
float CVector::CalcLineLineDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
	CVector* mp1, CVector* mp2, float* t1, float* t2)
{
	CVector v1 = e1 - s1;
	CVector v2 = e2 - s2;
	//2���������s
	if (v1.Cross(v2).Length() < 0.000001f)
	{
		//����1�̎n�_���璼��2�܂ł̍ŒZ�������ɋA������
		*t1 = 0.0f;
		*mp1 = s1;
		float dist = CalcPointLineDist(*mp1, s2, e2, mp2, t2);
		return dist;
	}
	//2���������s�łȂ�
	float dv1v2 = v1.Dot(v2);
	float dv1v1 = v1.Dot(v1);
	float dv2v2 = v2.Dot(v2);
	CVector vs2s1 = s1 - s2;
	//�䗦1�����߂�
	*t1 = (dv1v2 * v2.Dot(vs2s1) - dv2v2 * v1.Dot(vs2s1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	//��_1�����߂�
	*mp1 = s1 + v1 * *t1;
	//�䗦�����߂�
	*t2 = v2.Dot(*mp1 - s2) / dv2v2;
	//��_2�����߂�
	*mp2 = s2 + v2 * *t2;
	//�ŒZ������Ԃ�
	return (*mp2 - *mp1).Length();
}
//0~1�̊ԂɃN�����v(�l�������I�ɔ͈͓��ɂ���)
void CVector::Clamp0to1(float& v)
{
	if (v < 0.0f) v = 0.0f;
	else if (v > 1.0f) v = 1.0f;
}

//2�����Ԃ̍ŒZ������Ԃ�
float CVector::CalcSegmentSegmentDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2, CVector* mp1, CVector* mp2)
{
	float dist = 0, t1, t2;
	//�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
	dist = CalcLineLineDist(s1, e1, s2, e2, mp1, mp2, &t1, &t2);
	if (0.0f <= t1 && t1 <= 1.0f && 
		0.0f <= t2 && t2 <= 1.0f)
	{
		//mp1,mp2�������Ƃ��������ɂ�����
		return dist;
	}
	//mp1,mp2�̗����܂��͂ǂ��炩���������ɂȂ������̂Ŏ���
	
	//mp1,t1�����߂Ȃ���
	//t2��0~1�ɃN�����v����mp2����s1.v�ɐ��������낵�Ă݂�
	Clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	dist = CalcPointLineDist(*mp2, s1, e1, mp1, &t1);
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		//mp1���������ɂ�����
		return dist;
	}
	//mp1���������ɂȂ������̂Ŏ���

	//mp2,t2�����߂Ȃ���
	//t1��0~1�ɃN�����v����mp1����s2.v�ɐ��������낵�Ă݂�
	Clamp0to1(t1);
	*mp1 = s1 + (e1 - s1) * t1;
	dist = CalcPointLineDist(*mp1, s2, e2, mp2, &t2);
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		//mp2���������ɂ�����
		return dist;
	}
	//mp2���������ɂȂ������̂Ŏ�

	//t2���N�����v����mp2���Čv�Z����ƁAmp1����mp2�܂ł��ŒZ
	Clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	return (*mp2 - *mp1).Length();
}


//+���Z�q�̃I�[�o�[���[�h
//CVector + CVector�̉��Z���ʂ�Ԃ�
CVector CVector::operator+(const CVector& v)const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}
//-���Z�q�̃I�[�o�[���[�h
//CVector - Cvector�̉��Z���ʂ�Ԃ�
CVector CVector::operator-(const CVector& v)const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}
//*���Z�q�̃I�[�o�[���[�h
//CVector * Cfloat�̉��Z���ʂ�Ԃ�
CVector CVector::operator*(const float& f)const
{
	return CVector(mX * f, mY * f, mZ * f);
}
//*���Z�q�̃I�[�o�[���[�h
//CVector * CMatrix�̉��Z���ʂ�CVector�ŕԂ�
CVector CVector::operator*(const CMatrix& m)const
{
	//�|���Z�̌��ʂ�CVector�^�̒l�ŕԂ�
	return CVector(
		mX * m.GetM(0, 0) + mY * m.GetM(1, 0) + mZ * m.GetM(2, 0) + m.GetM(3, 0),
		mX * m.GetM(0, 1) + mY * m.GetM(1, 1) + mZ * m.GetM(2, 1) + m.GetM(3, 1),
		mX * m.GetM(0, 2) + mY * m.GetM(1, 2) + mZ * m.GetM(2, 2) + m.GetM(3, 2)
		);
}
//+=���Z�q�̃I�[�o�[���[�h
//CVector1 += CVector2�̉��Z���s��
void CVector::operator+=(const CVector& v)
{
	mX += v.GetX();
	mY += v.GetY();
	mZ += v.GetZ();
}

//Set(X���W,Y���W,Z���W)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}
//X�̒l��ݒ�
void CVector::SetX(float f)
{
	mX = f;
}
//Y�̒l��ݒ�
void CVector::SetY(float f)
{
	mY = f;
}
//Z�̒l��ݒ�
void CVector::SetZ(float f)
{
	mZ = f;
}
//X�̒l��Ԃ�
float CVector::GetX() const
{
	return mX;
}
//Y�̒l��Ԃ�
float CVector::GetY() const
{
	return mY;
}
//Z�̒l��Ԃ�
float CVector::GetZ() const
{
	return mZ;
}
