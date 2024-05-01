#pragma once
#include "CCharacter3.h"
#include "CTransform.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CModel.h"
#include "CVector.h"
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CEnemy3.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUi.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CXPlayer.h"

#include "CSound.h"

class CApplication
{
public:
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//Textureクラスのインスタンスを取得
	static CTexture* GetTexture();
	//UIクラスのインスタンスを取得
	static CUi* GetUi();
	//デストラクタ
	~CApplication();

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
	CModelX mModelX;
	CColliderMesh mColliderMesh;
	CMatrix mMatrix;

	CPlayer mPlayer;
	CXPlayer mXPlayer;

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