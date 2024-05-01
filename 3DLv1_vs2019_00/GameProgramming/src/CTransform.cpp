#include "CTransform.h"

const CVector& CTransform::GetPosition() const
{
	return mPosition;
}

void CTransform::SetPosition(const CVector& v)
{
	mPosition = v;
}

void CTransform::SetRotation(const CVector& v)
{
	mRotation = v;
}

void CTransform::SetScale(const CVector& v)
{
	mScale = v;
}

const CMatrix& CTransform::GetMatrix() const
{
	return mMatrix;
}

const CMatrix& CTransform::GetMatrixRotate() const
{
	return mMatrixRotate;
}

void CTransform::Update(const CVector& pos, const CVector& rot, const CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	Update();
}

//行列更新処理
void CTransform::Update()
{
	//拡大縮小行列の設定
	mMatrixScale.SetScale(mScale.GetX(), mScale.GetY(), mScale.GetZ());
	//回転行列の設定
	mMatrixRotate = 
		CMatrix().SetRotateZ(mRotation.GetZ()) *
		CMatrix().SetRotateX(mRotation.GetX()) * 
		CMatrix().SetRotateY(mRotation.GetY());
	//平行移動行列の設定
	mMatrixTranslate.SetTranlate(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());
	//合成行列の設定
	mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;
}