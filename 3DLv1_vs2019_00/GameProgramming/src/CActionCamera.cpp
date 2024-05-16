#include "CActionCamera.h"
#include "glut.h"
#include <iostream>

#define TURN_V 1.0f//回転速度

CActionCamera* CActionCamera::spInstance = nullptr;

CActionCamera::CActionCamera()
{
	spInstance = this;
}

//インスタンスの取得
CActionCamera* CActionCamera::GetInstance()
{
	return spInstance;
}

//設定メソッド
void CActionCamera::Set(float distance, float xaxis, float yaxis)
{
	SetRotation(CVector(xaxis, yaxis, 0.0f));
	SetScale(CVector(0.0f, 0.0f, distance));
	mUp = CVector(0.0f, 1.0f, 0.0f);
	spInstance = this;
}

//カメラ更新
void CActionCamera::Update()
{
	if (mInput.Key('J'))
	{
		mRotation = mRotation + CVector(0.0f, TURN_V, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mRotation = mRotation - CVector(0.0f, TURN_V, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mRotation = mRotation - CVector(TURN_V, 0.0f, 0.0f);
		if (mRotation.GetX() < -80.0f)
		{
			mRotation.SetX(-80.0f);
		}
	}
	if (mInput.Key('K'))
	{
		mRotation = mRotation + CVector(TURN_V, 0.0f, 0.0f);
		if (mRotation.GetX() > 80.0f)
		{
			mRotation.SetX(80.0f);
		}
	}
	CTransform::Update();
	mCenter = mPosition;
	mEye = mPosition + mMatrixRotate.GetVectorZ() * mScale.GetZ();
}
//カメラ適用
void CActionCamera::Render()
{
	gluLookAt(mEye.GetX(), mEye.GetY(), mEye.GetZ(),
		mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(),
		mUp.GetX(), mUp.GetY(), mUp.GetZ());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelView.GetM());
}

CVector CActionCamera::GetVectorX()
{
	return CVector(-mModelView.GetM(0, 0), -mModelView.GetM(1, 0), -mModelView.GetM(2, 0));
}

CVector CActionCamera::GetVectorZ()
{
	return CVector(-mModelView.GetM(0, 2), -mModelView.GetM(1, 2), -mModelView.GetM(2, 2));
}