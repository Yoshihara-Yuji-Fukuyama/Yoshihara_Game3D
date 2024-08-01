#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//クラスのstatic変数
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;
CUi* CApplication::spUi = nullptr;
CApplication* CApplication::spInstance = nullptr;

//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"//背景モデル
#define MODEL_STAGE "res\\StageWall.obj","res\\StageWall.mtl" //ステージモデル
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"       //プレイヤーモデル
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"          //敵輸送機モデル
#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

#define MAX_ENEMY 1//敵の最大数

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

//UIクラスのインスタンスを取得
CUi* CApplication::GetUi()
{
	return spUi;//インスタンスのポインタを返す
}

//インスタンスの取得
CApplication* CApplication::GetInstance()
{
	return spInstance;
}

//デストラクタ
CApplication::~CApplication()
{
	delete spUi;//インスタンスUiの削除
}

//初期設定
void CApplication::Init()
{
	//プレイヤーの初期設定
	mXPlayer.Init(&mPlayerModel);

	//パラディンのインスタンス作成
	//mpPaladin = new CPaladin();
	//攻撃アニメーションに変更
	//mpPaladin->ChangeAnimation(1, true, 50);
	
	mEnemyNum = 0;//敵の数は0
}

void CApplication::Start()
{
	//インスタンスに設定
	spInstance = this;
	//プレイヤー3Dモデルファイル読み込み
	mPlayerModel.Load(MODEL_PLAYER);//0:前歩き
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
	mPlayerModel.AddAnimationSet(HIT);//12:被弾
	mPlayerModel.AddAnimationSet(WALK_HIT);//13:歩きながら被弾
	mPlayerModel.AddAnimationSet(DEATH);//14:死亡
	mPlayerModel.AddAnimationSet(ROLL);//15:ローリング
	//背景モデル
	mBackGround.Load(MODEL_BACKGROUND);
	//ステージモデル
	//mStage.Load(MODEL_STAGE);
	//フォントのロード
	mFont.Load("FontG.png", 1, 4096 / 64);
	//カメラの設定
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//CApplicationのInit()
	Init();
	
	//TODO:ステージの設置
	//背景モデルから三角コライダを生成
	//親インスタンスと親行列はなし
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
	//mColliderMesh.Set(nullptr, nullptr, &mStage);
	//パラディンの配置
	//mpPaladin->SetPosition(CVector(-1.0f, 0.0f, 5.0f));
	//mpPaladin->SetScale(CVector(0.025f, 0.025f, 0.025f));
}

void CApplication::Update()
{	
	SpawnEnemy();

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


	mBackGround.Render();
	//mStage.Render();
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
	if (CActionCamera::GetInstance()->WorldToScreen(&screen, mpXEnemy[0]->GetPosition()))
	{
		//変換先の座標に文字列を出力する
		mFont.Draw(screen.GetX(), screen.GetY(), 7, 14, "ENEMY");
	}
	
	//2Dの描画終了
	CCamera::End();
}

void CApplication::SpawnEnemy()
{
	//制限を付ける
	//敵の最大数まで生成する
	while (mEnemyNum < MAX_ENEMY)
	{
		//X座標-100から100、Y座標0、Z座標-100から100にランダムに生成
		mpXEnemy.push_back(new CXEnemy(CVector(rand() % 200 - 100, 0.0f, rand() % 200 - 100)));
		mEnemyNum++;
	}
}

void CApplication::DeathEnemy()
{
	mEnemyNum--;
}

