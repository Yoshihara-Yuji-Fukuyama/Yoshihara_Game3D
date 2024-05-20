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

//点と線(始点、終点を通る直線)の最短距離を求める
float CVector::CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t)
{
	*t = 0.0f;//割合の初期化
	CVector v = e - s;//始点から終点へのベクトルを求める
	float dvv = v.Dot(v);//ベクトルの長さの2乗を求める
	if (dvv > 0.0f)
	{
		*t = v.Dot(p - s) / dvv;//線上の垂線となる点の割合を求める
		//上の式の説明
		//dot(v , p - sp)は|v||p - sp|cosθ
		//dvvは|v|の2乗
		//上の計算で、tは |p - sp|cosθ / |v|となる
		//つまりtは「dotで投影した長さ÷vの長さ」という割合になる
	}
	*mp = s + v * *t;//線上の垂線となる点を求める
	return (p - *mp).Length();//垂線の長さを返す
}

//2線間の最短距離を返す
float CVector::CalcLineLineDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
	CVector* mp1, CVector* mp2, float* t1, float* t2)
{
	CVector v1 = e1 - s1;
	CVector v2 = e2 - s2;
	//2直線が平行
	if (v1.Cross(v2).Length() < 0.000001f)
	{
		//線分1の始点から直線2までの最短距離問題に帰着する
		*t1 = 0.0f;
		*mp1 = s1;
		float dist = CalcPointLineDist(*mp1, s2, e2, mp2, t2);
		return dist;
	}
	//2直線が平行でない
	float dv1v2 = v1.Dot(v2);
	float dv1v1 = v1.Dot(v1);
	float dv2v2 = v2.Dot(v2);
	CVector vs2s1 = s1 - s2;
	//比率1を求める
	*t1 = (dv1v2 * v2.Dot(vs2s1) - dv2v2 * v1.Dot(vs2s1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);
	//交点1を求める
	*mp1 = s1 + v1 * *t1;
	//比率を求める
	*t2 = v2.Dot(*mp1 - s2) / dv2v2;
	//交点2を求める
	*mp2 = s2 + v2 * *t2;
	//最短距離を返す
	return (*mp2 - *mp1).Length();
}
//0~1の間にクランプ(値を強制的に範囲内にする)
void CVector::Clamp0to1(float& v)
{
	if (v < 0.0f) v = 0.0f;
	else if (v > 1.0f) v = 1.0f;
}

//2線分間の最短距離を返す
float CVector::CalcSegmentSegmentDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2, CVector* mp1, CVector* mp2)
{
	float dist = 0, t1, t2;
	//とりあえず2直線間の最短距離,mp1,mp2,t1,t2を求めてみる
	dist = CalcLineLineDist(s1, e1, s2, e2, mp1, mp2, &t1, &t2);
	if (0.0f <= t1 && t1 <= 1.0f && 
		0.0f <= t2 && t2 <= 1.0f)
	{
		//mp1,mp2が両方とも線分内にあった
		return dist;
	}
	//mp1,mp2の両方またはどちらかが線分内になかったので次へ
	
	//mp1,t1を求めなおす
	//t2を0~1にクランプしてmp2からs1.vに垂線をおろしてみる
	Clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	dist = CalcPointLineDist(*mp2, s1, e1, mp1, &t1);
	if (0.0f <= t1 && t1 <= 1.0f)
	{
		//mp1が線分内にあった
		return dist;
	}
	//mp1が線分内になかったので次へ

	//mp2,t2を求めなおす
	//t1を0~1にクランプしてmp1からs2.vに垂線をおろしてみる
	Clamp0to1(t1);
	*mp1 = s1 + (e1 - s1) * t1;
	dist = CalcPointLineDist(*mp1, s2, e2, mp2, &t2);
	if (0.0f <= t2 && t2 <= 1.0f)
	{
		//mp2が線分内にあった
		return dist;
	}
	//mp2が線分内になかったので次

	//t2をクランプしてmp2を再計算すると、mp1からmp2までが最短
	Clamp0to1(t2);
	*mp2 = s2 + (e2 - s2) * t2;
	return (*mp2 - *mp1).Length();
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
CVector CVector::operator*(const CMatrix& m)const
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
