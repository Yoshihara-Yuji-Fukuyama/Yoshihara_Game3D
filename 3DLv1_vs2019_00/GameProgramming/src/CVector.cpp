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
CVector CVector::operator*(const CMatrix& m)
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