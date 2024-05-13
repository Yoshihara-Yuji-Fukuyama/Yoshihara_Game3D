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

#include "CTransform.h"
#include "CMatrix.h"
#include "CVector.h"
#include <vector>

#include "CInput.h"
#include "CFont.h"
#include "CUi.h"
#include "CSound.h"

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

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

private:
	static CMatrix mModelViewInverse;//モデルビューの逆行列
	static CUi* spUi;                //Uiクラスのポインタ
	static CTexture mTexture;        //Textureクラスのインスタンス
	CModel mModel;
	CModel mModelC5;
	CModel mBackGround;
	CModelX mPlayerModel; //プレイヤーのモデル
	CModelX mKnightModel; //Knightのモデル

	CColliderMesh mColliderMesh;
	CMatrix mMatrix;

	CXPlayer mXPlayer;
	CXEnemy mXEnemy;

	CPlayer* mpPlayer;
	CEnemy* mpEnemy;

	CInput mInput;
	CFont mFont;

	CVector mEye; //カメラの視点

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