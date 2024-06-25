#pragma once
#include "CModel.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CCharacter3.h"
#include "CTexture.h"
#include "CTriangle.h"
#include "CBillBoard.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CTaskManager.h"

#include "CPlayer.h"
#include "CEnemy.h"
#include "CEnemy3.h"
#include "CBullet.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CPaladin.h"
#include "CActionCamera.h"

#include "CTransform.h"
#include "CMatrix.h"
#include "CVector.h"
#include <vector>

#include "CInput.h"
#include "CFont.h"
#include "CUi.h"
#include "CSound.h"

#include "CWepon.h"
class CApplication
{
public:
	//モデルビュー行列の取得
	static const CMatrix& GetModelViewInverse();
	//Textureクラスのインスタンスを取得
	static CTexture* GetTexture();
	//UIクラスのインスタンスを取得
	static CUi* GetUi();
	//デストラクタ
	~CApplication();
	//初期設定
	void Init();

	void Start();
	void Update();

	void SpawnEnemy();
private:
	static CMatrix mModelViewInverse;//モデルビューの逆行列
	static CUi* spUi;                //Uiクラスのポインタ
	static CTexture mTexture;        //Textureクラスのインスタンス
	CModel mModel;
	CModel mModelC5;
	CModel mBackGround;//背景のモデル
	CModelX mPlayerModel; //プレイヤーのモデル

	CColliderTriangle mColliderTriangle;
	CColliderMesh mColliderMesh;//モデルからコライダを生成
	CMatrix mMatrix;

	CXPlayer mXPlayer;
	std::vector<CXEnemy*> mpXEnemy;

	CEnemy3* mpEnemy3;

	CPlayer* mpPlayer;
	CEnemy* mpEnemy;
	CPaladin* mpPaladin;

	CInput mInput;
	CFont mFont;

	CVector mEye; //カメラの視点
	CActionCamera mActionCamera;//CActionCamera

	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー音

	//ゲームの状態
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};
	EState mState;

	CWepon* mWepon;
};