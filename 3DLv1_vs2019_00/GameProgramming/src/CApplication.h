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
#include "CUiTexture.h"
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
	//インスタンスの取得
	static CApplication* GetInstance();
	//デストラクタ
	~CApplication();
	//初期設定
	void Init();

	void Start();
	void Update();

	//敵を出現させる
	void SpawnEnemy();
	//mEnemyNumを減らす
	void DeathEnemy();
private:
	static CMatrix mModelViewInverse;//モデルビューの逆行列
	static CUi* spUi;                //Uiクラスのポインタ
	static CTexture mTexture;        //Textureクラスのインスタンス
	static CApplication* spInstance;//CApplicationのインスタンス

	CModel mBackGround;//背景のモデル
	CModel mStage;//ステージのモデル
	CModelX mPlayerModel; //プレイヤーのモデル

	CColliderTriangle mColliderTriangle;
	CColliderMesh mColliderMesh;//モデルからコライダを生成
	CMatrix mMatrix;

	CXPlayer mXPlayer;
	std::vector<CXEnemy*> mpXEnemy;
	int mEnemyNum;//敵の出現している数

	CInput mInput;
	CFont mFont;
	CUiTexture* mpHpBar;
	CUiTexture* mpEnemyHpBar;

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
};