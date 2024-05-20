
#ifndef CVECTOR_H
#define CVECTOR_H
#include "CMatrix.h"

/*
ベクトルクラス
ベクトルデータを扱います
*/
class CVector 
{
public:
	CVector();
	//コンストラクタ
	//CVector(X座標,Y座標,Z座標)
	CVector(float x, float y, float z);

	//正規化
	//大きさ1のベクトルを返す
	CVector Normalize() const;
	//外積
	//Cross(ベクトル)
	CVector Cross(const CVector& v)const;
	//内積
	//Dot(ベクトル)
	float Dot(const CVector& v)const;
	//ベクトルの長さを返す
	float Length() const;
	
	/// <summary>
	/// 点と線(始点、終点を通る直線)の最短距離を求める
	/// </summary>
	/// <param name="p">点</param>
	/// <param name="s">始点</param>
	/// <param name="e">終点</param>
	/// <param name="mp">戦場の最短点</param>
	/// <param name="t">割合</param>
	/// <returns>垂線の長さ</returns>
	float CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t);
	/// <summary>
	/// 2線間の最短距離を返す
	/// </summary>
	/// <param name="s1">始点1</param>
	/// <param name="e1">終点1</param>
	/// <param name="s2">始点2</param>
	/// <param name="e2">終点2</param>
	/// <param name="mp1">交点1</param>
	/// <param name="mp2">交点2</param>
	/// <param name="t1">比率1</param>
	/// <param name="t2">比率2</param>
	/// <returns>2線間の最短距離</returns>
	float CalcLineLineDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
		CVector* mp1, CVector* mp2, float* t1, float* t2);
	//0~1の間にクランプ(値を強制的に範囲内にする)
	void Clamp0to1(float& v);
	/// <summary>
	/// 2線分間の最短距離を返す
	/// </summary>
	/// <param name="s1">始点1</param>
	/// <param name="e1">終点1</param>
	/// <param name="s2">始点2</param>
	/// <param name="e2">終点2</param>
	/// <param name="mp1">最短線の端点1</param>
	/// <param name="mp2">最短線の端点2</param>
	/// <returns>2線分間の最短距離</returns>
	float CalcSegmentSegmentDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
		CVector* mp1, CVector* mp2);
		

	//+演算子のオーバーロード
    //CVector + CVectorの演算結果を返す
	CVector operator+(const CVector& v)const;
	//-演算子のオーバーロード
	//CVector - CVectorの演算結果を返す
	CVector operator-(const CVector& v)const;
	//*演算子のオーバーロード
    //CVector * floatの演算結果を返す
	CVector operator*(const float& f)const;
	//CVector * CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix& m)const;
	//+=演算子のオーバーロード
	//CVector1 += CVector2の演算を行う
	void operator+=(const CVector& v);

	//各軸での値の設定
	//Set(X座標, Y座標, Z座標)
	void Set(float x, float y, float z);
	//Xの値を設定
	void SetX(float f);
	//Yの値を設定
	void SetY(float f);
	//Zの値を設定
	void SetZ(float f);
	//Xの値を返す
	float GetX() const;
	//Yの値を返す
	float GetY() const;
	//Zの値を返す
	float GetZ() const;
private:
	//3D各軸での値を設定
	float mX, mY, mZ;
};
#endif