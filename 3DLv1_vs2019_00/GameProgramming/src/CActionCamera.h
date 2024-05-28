#ifndef CACTIONCAMERA_H
#define CACTIONCAMERA_H

#include<GLFW/glfw3.h>
#include <Windows.h>
#include "CVector.h"
#include "CTransform.h"
#include "CMatrix.h"
#include "CInput.h"
#include "CXPlayer.h"

//カメラクラス
class CActionCamera : public CTransform
{
public:
	CActionCamera();
	//インスタンスの取得
	static CActionCamera* GetInstance();

	/// <summary>
	/// 設定メソッド
	/// </summary>
	/// <param name="distance">距離</param>
	/// <param name="xaxis">X回転</param>
	/// <param name="yaxis">Y回転</param>
	void Set(float distance, float xaxis, float yaxis);
	//カメラ更新
	void Update();
	//カメラ適用
	void Render();
	//カメラのX軸取得
	CVector GetVectorX();
	//カメラのZ軸取得
	CVector GetVectorZ();
	/// <summary>
	/// ワールド座標をスクリーン座標へ変換する
	/// </summary>
	/// <param name="screen">スクリーン座標</param>
	/// <param name="world">ワールド座標</param>
	/// <returns></returns>
	bool WorldToScreen(CVector* screen, const CVector& world);
	//マウス感度を返す
	float GetSensitivity();
private:
	static CActionCamera* spInstance;//インスタンス
	CVector mEye;	//視点の位置
	CVector mCenter;//注視点
	CVector mUp;	//上方向
	CMatrix mModelView;//モデルビュー行列
	CInput mInput;//入力クラス
	int mScreenWidth; //幅
	int mScreenHeight;//高さ
	CMatrix mProjection;//プロジェクション行列
	float mTurnVertical;//縦の回転量
	float mTurnHorizontal;//横の回転量
	float mSensitivity;//マウス感度
	POINT mScreenCenter;//画面の中央
};
#endif