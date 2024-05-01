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
const CMatrix& CApplication::ModelViewInverse()
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

void CApplication::Start()
{
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{	
	//最初のアニメーションの現在時間を設定する
	mModelX.GetAnimationSet()[0]->SetTime(
		mModelX.GetAnimationSet()[0]->GetTime() + 1.0f);
	mModelX.GetAnimationSet()[0]->SetTime(
		(int)mModelX.GetAnimationSet()[0]->GetTime() %
		(int)(mModelX.GetAnimationSet()[0]->GetMaxTime() + 1));
	//最初のアニメーションの重みを1.0(100%)にする
	mModelX.GetAnimationSet()[0]->SetWeight(1.0f);
	//フレームの変換行列をアニメーションで更新する
	mModelX.AnimateFrame();
	//フレームの合成行列を計算する
	mModelX.GetFrames()[0]->SetAnimateCombined(&mMatrix);

	//カメラのパラメータを作成する
	CVector  e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X軸＋回転
	if (mInput.Key('K'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//X軸－回転
	if (mInput.Key('I'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y軸＋回転
	if (mInput.Key('L'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y軸－回転
	if (mInput.Key('J'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//行列設定
	glMultMatrixf(mMatrix.M());

	//頂点にアニメーションを適用する
	mModelX.AnimateVertex();
	//モデル描画
	mModelX.Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	//2Dの描画終了
	CCamera::End();
}

