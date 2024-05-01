#include "CVector.h"
#include <math.h>

CVector::CVector()
	:mX(0.0f)
	, mY(0.0f)
	, mZ(0.0f)
{}
//コンストラクタ
CVector::CVector(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{}

//正規化
CVector CVector::Normalize() const
{
	//ベクトルの大きさで割ったベクトルを返す（長さ1のベクトル）
	return *this * (1.0f / Length());
}
//内積
float CVector::Dot(const CVector& v)const
{
	return mX * v.mX + mY * v.mY + mZ * v.mZ;
}
//外積
CVector CVector::Cross(const CVector& v)const
{
	return CVector(mY * v.mZ - mZ * v.mY, mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX);
}
//ベクトルの長さを返す
float CVector::Length() const
{
	//sqrt関数で平方根を返す
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

//+演算子のオーバーロード
//CVector + CVectorの演算結果を返す
CVector CVector::operator+(const CVector& v)const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}
//-演算子のオーバーロード
//CVector - Cvectorの演算結果を返す
CVector CVector::operator-(const CVector& v)const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}
//*演算子のオーバーロード
//CVector * Cfloatの演算結果を返す
CVector CVector::operator*(const float& f)const
{
	return CVector(mX * f, mY * f, mZ * f);
}
//*演算子のオーバーロード
//CVector * CMatrixの演算結果をCVectorで返す
CVector CVector::operator*(const CMatrix& m)
{
	//掛け算の結果をCVector型の値で返す
	return CVector(
		mX * m.GetM(0, 0) + mY * m.GetM(1, 0) + mZ * m.GetM(2, 0) + m.GetM(3, 0),
		mX * m.GetM(0, 1) + mY * m.GetM(1, 1) + mZ * m.GetM(2, 1) + m.GetM(3, 1),
		mX * m.GetM(0, 2) + mY * m.GetM(1, 2) + mZ * m.GetM(2, 2) + m.GetM(3, 2)
		);
}
//+=演算子のオーバーロード
//CVector1 += CVector2の演算を行う
void CVector::operator+=(const CVector& v)
{
	mX += v.GetX();
	mY += v.GetY();
	mZ += v.GetZ();
}

//Set(X座標,Y座標,Z座標)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}
//Xの値を設定
void CVector::SetX(float f)
{
	mX = f;
}
//Yの値を設定
void CVector::SetY(float f)
{
	mY = f;
}
//Zの値を設定
void CVector::SetZ(float f)
{
	mZ = f;
}
//Xの値を返す
float CVector::GetX() const
{
	return mX;
}
//Yの値を返す
float CVector::GetY() const
{
	return mY;
}
//Zの値を返す
float CVector::GetZ() const
{
	return mZ;
}