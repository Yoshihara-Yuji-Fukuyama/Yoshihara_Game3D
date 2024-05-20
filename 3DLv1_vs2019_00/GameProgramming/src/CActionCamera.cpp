#include "CActionCamera.h"
#include "glut.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "CXPlayer.h"

CActionCamera* CActionCamera::spInstance = nullptr;

CActionCamera::CActionCamera()
	: mFirstMouse(true)
	, mLastX(400.0f)//初期マウス位置は中央
	, mLastY(300.0f)
	, mTurnVertical(0.0f)//初期回転量は0
	, mTurnHorizontal(0.0f)
	, mSensitivity(0.1f)//初期感度
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

	int viewport[4];
	//現在のビューポートを保存しておく
	glGetIntegerv(GL_VIEWPORT, viewport);
	mScreenWidth = viewport[2]; //幅を取得
	mScreenHeight = viewport[3];//高さを取得
	//プロジェクション行列の取得
	glGetFloatv(GL_PROJECTION_MATRIX, mProjection.GetM());
}

//マウスの移動のコールバック関数
//マウスが移動すると前の座標との差分を設定する
void CActionCamera::MouseCallback(double xpos, double ypos)
{
	//マウス操作が初めてなら
	if (mFirstMouse)
	{
		//前の座標に今の座標を入れる
		mLastX = xpos;
		mLastY = ypos;
		mFirstMouse = false;
	}
	//X座標の差
	float xoffset = xpos - mLastX;
	//Y座標の差
	float yoffset = ypos - mLastY;
	//前の座標を更新する
	mLastX = xpos;
	mLastY = ypos;
	//座標の差と感度を掛けた数値を回転量に設定
	mTurnVertical = yoffset * mSensitivity;//縦の回転量
	mTurnHorizontal = xoffset * mSensitivity;//横の回転量
}

//カメラ更新
void CActionCamera::Update()
{
	//回転量の分だけ回転させる
	mRotation = mRotation - CVector(mTurnVertical, mTurnHorizontal, 0.0f);
	//CXPlayer::GetInstance()->mRotation;
	CTransform::Update();
	//注視点の設定
	mCenter = mPosition;
	//視点の位置の設定
	mEye = mPosition + mMatrixRotate.GetVectorZ() * mScale.GetZ();
	//回転量を0にする
	mTurnVertical = 0.0f;
	mTurnHorizontal = 0.0f;
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

//カメラのX軸取得
CVector CActionCamera::GetVectorX()
{
	return CVector(-mModelView.GetM(0, 0), -mModelView.GetM(1, 0), -mModelView.GetM(2, 0));
}
//カメラのZ軸取得
CVector CActionCamera::GetVectorZ()
{
	return CVector(-mModelView.GetM(0, 2), -mModelView.GetM(1, 2), -mModelView.GetM(2, 2));
}

//ワールド座標をスクリーン座標へ変換する
bool CActionCamera::WorldToScreen(CVector* screen, const CVector& world)
{
	//座標変換
	CVector modelview_pos = world * mModelView;
	//画面外なのでリターン
	if (modelview_pos.GetZ() >= 0.0f)
	{
		return false;
	}

	//座標調整
	CVector screen_pos = modelview_pos * mProjection;
	screen_pos = screen_pos * (1.0f / -modelview_pos.GetZ());

	//スクリーン変換
	screen->SetX((1.0f + screen_pos.GetX()) * mScreenWidth * 0.5f);
	screen->SetY((1.0f + screen_pos.GetY()) * mScreenHeight * 0.5f);
	screen->SetZ(screen_pos.GetZ());

	return true;
}
