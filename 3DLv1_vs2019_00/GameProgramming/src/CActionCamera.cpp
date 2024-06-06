#include "CActionCamera.h"
#include "glut.h"
#include <iostream>
#include "CXPlayer.h"
#define TURN_V 1.0f	//回転速度

CActionCamera* CActionCamera::spInstance = nullptr;

CActionCamera::CActionCamera()
	: mTurnVertical(0.0f)//初期回転量は0
	, mTurnHorizontal(0.0f)
	, mSensitivity(0.1f)//初期感度
{
	spInstance = this;

	//画面の中央を取得
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	mScreenCenter = { screenWidth / 2 , screenHeight / 2 };
	//カーソルを非表示
	ShowCursor(FALSE);
	//画面の中央にカーソルを設定
	SetCursorPos(mScreenCenter.x, mScreenCenter.y);
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

//カメラ更新
void CActionCamera::Update()
{	
	//現在のカーソル位置を取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	//カーソルの移動量を計算
	int deltaX = cursorPos.x - mScreenCenter.x;
	int deltaY = cursorPos.y - mScreenCenter.y;
	//回転量を計算
	mTurnVertical = deltaY * mSensitivity;
	mTurnHorizontal = deltaX * mSensitivity;

	//回転量の分だけ回転する
	mRotation = mRotation - CVector(mTurnVertical, mTurnHorizontal, 0.0f);
	CXPlayer::GetInstance()->Turn(mTurnHorizontal);
	//行列を更新
	CTransform::Update();
	//注視点の設定
	mCenter = mPosition;
	//視点の位置の設定
	mEye = mPosition + mMatrixRotate.GetVectorZ() * mScale.GetZ();
	//カーソルを中央に戻す
	SetCursorPos(mScreenCenter.x, mScreenCenter.y);
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

//マウス感度を返す
float CActionCamera::GetSensitivity()
{
	return mSensitivity;
}

//回転を取得する
CVector CActionCamera::GetRotation()
{
	return mRotation;
}
