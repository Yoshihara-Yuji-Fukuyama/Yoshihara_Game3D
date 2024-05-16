#ifndef CACTIONCAMERA_H
#define CACTIONCAMERA_H

#include "CVector.h"
#include "CTransform.h"
#include "CMatrix.h"
#include "CInput.h"

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
private:
	//インスタンス
	static CActionCamera* spInstance;
	//視点
	CVector mEye;
	//注視点
	CVector mCenter;
	//上方向
	CVector mUp;
	//モデルビュー行列
	CMatrix mModelView;
	//入力クラス
	CInput mInput;
};
#endif