#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//クラスのstatic変数
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;

//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"//背景モデル
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"       //プレイヤーモデル
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"          //敵輸送機モデル
#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

//モデルビュー行列の取得
const CMatrix& CApplication::GetModelViewInverse()
{
	return mModelViewInverse;
}

//Textureクラスのインスタンスを取得
CTexture* CApplication::GetTexture()
{
	return &mTexture;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::GetUi()
{
	return spUi;//インスタンスのポインタを返す
}

//デストラクタ
CApplication::~CApplication()
{
	delete spUi;//インスタンスUiの削除
}

//初期設定
void CApplication::Init()
{
	//3Dモデルファイル読み込み
	mPlayerModel.Load(MODEL_PLAYER);//0:前歩き
	mKnightModel.Load(MODEL_KNIGHT);
	//追加アニメーション読み込み
	mPlayerModel.AddAnimationSet(BACKWARD); //1:後ろ歩き
	mPlayerModel.AddAnimationSet(L_WALK);   //2:左歩き
	mPlayerModel.AddAnimationSet(R_WALK);   //3:右歩き
	mPlayerModel.AddAnimationSet(AIM_IDLE); //4:構え待機
	mPlayerModel.AddAnimationSet(Fire);     //5:射撃
	mPlayerModel.AddAnimationSet(JUMP_UP);  //6:ジャンプ
	mPlayerModel.SeparateAnimationSet(6, 5, 16, "JumpUp");//7:ジャンプ上昇
	mPlayerModel.AddAnimationSet(JUMP_DOWN);//8:ジャンプ降下
	mPlayerModel.AddAnimationSet(RUN);//9:走り
	mPlayerModel.AddAnimationSet(IDLE_RELOAD);//10:止まってリロード
	mPlayerModel.AddAnimationSet(WALK_RELOAD);//11:歩きながらリロード
	//パラディンのインスタンス作成
	mpPaladin = new CPaladin();
	//敵のアニメーションを抜き出す
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//1:移動
	mKnightModel.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:待機
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//3:ダミー
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//4:ダミー
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//5:ダミー
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//6:ダミー
	mKnightModel.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	mKnightModel.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//9:ダミー
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//10:ダミー
	mKnightModel.SeparateAnimationSet(0, 1160, 1260, "death1");//11:ダウン
	
	//背景モデル
	mBackGround.Load(MODEL_BACKGROUND);
	//プレイヤーの初期設定
	mXPlayer.Init(&mPlayerModel);
	
	//敵の初期設定
	mXEnemy.Init(&mKnightModel);
	//待機アニメーションに変更
	mXEnemy.ChangeAnimation(2, true, 200);
	//攻撃アニメーションに変更
	mpPaladin->ChangeAnimation(1, true, 50);
	
	//フォントのロード
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Start()
{
	//カメラの設定
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//CApplicationのInit()
	Init();
	
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列はなし
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
	/*mColliderTriangle.Set(nullptr, nullptr
		, CVector(-50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f));*/
	//TODO:地面の当たり判定を出す
	//敵の配置
	mXEnemy.SetPosition(CVector(7.0f, 0.0f, 0.0f));
	//パラディンの配置
	mpPaladin->SetPosition(CVector(-1.0f, 0.0f, 5.0f));
	mpPaladin->SetScale(CVector(0.025f, 0.025f, 0.025f));
}

void CApplication::Update()
{	
	/*
	//プレイヤーの更新
	mXPlayer.Update();
	mXPlayer.WeponUpdate();
	//敵の更新
	mXEnemy.Update();
	//パラディンの更新
	mpPaladin->Update();
	mpPaladin->WeponUpdate();
	*/
	CTaskManager::GetInstance()->Update();
	
	mActionCamera.SetPosition(CVector(mXPlayer.GetPosition().GetX(), mXPlayer.GetPosition().GetY() + 5.0f, mXPlayer.GetPosition().GetZ()));
	mActionCamera.Update();
	mActionCamera.Render();
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	/*
	//プレイヤー描画
	mXPlayer.Render();
	mXPlayer.WeponRender();//プレイヤーの武器描画
	//敵描画
	mXEnemy.Render();
	//パラディンの描画
	mpPaladin->Render();
	mpPaladin->WeponRender();
	*/
	mBackGround.Render();
	CTaskManager::GetInstance()->Render();
	CTaskManager::GetInstance()->Delete();

	//コライダの描画
	CCollisionManager::GetInstance()->Render();
	//衝突処理
	CCollisionManager::GetInstance()->Collision();


	//2D描画開始
	CCamera::Start(0, 1920, 0, 1080);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");
	
	CVector screen;
	//Enemyの座標をスクリーン座標へ変換する
	if (CActionCamera::GetInstance()->WorldToScreen(&screen, mXEnemy.GetPosition()))
	{
		//変換先の座標に文字列を出力する
		mFont.Draw(screen.GetX(), screen.GetY(), 7, 14, "ENEMY");
	}
	
	//2Dの描画終了
	CCamera::End();
}

