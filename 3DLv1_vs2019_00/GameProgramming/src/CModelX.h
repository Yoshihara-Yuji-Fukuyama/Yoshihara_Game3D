#ifndef CMODELX_H //インクルードガード
#define CMODELX_H

#include <vector>//vectorクラスのインクルード（動的配列）
#include "CVector.h"
#include "CMatrix.h"
#include "CMaterial.h"
class CModelX;      //CModelXクラスの宣言
class CModelXFrame; //CModelXFrameクラスの宣言
class CMesh;        //CMeshクラスの宣言
class CSkinWeights; //CSkinWeightsクラスの宣言
class CAnimationSet;//CAnimationSetクラスの宣言
class CAnimation;   //CAnimationクラスの宣言
class CAnimationKey;//CAnimationKeyクラスの宣言
class CMaterial;    //マテリアルの宣言


#define MODEL_FILE "res\\ラグナ.x" //入力ファイル名

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a) delete[] a; a = nullptr;}
//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX {
	friend CModelXFrame;
	friend CAnimationSet;
	friend CAnimation;
public:
	CModelX();
	~CModelX();

	//ノードの読み飛ばし
	void SkipNode();
	//単語の取り出し
	char* GetToken();
	//mTokenのポインタを返すのみ
	char* GetOnlyToken();
	//ファイル読み込み
	void Load(char* file);
	//描画
	void Render();
	//トークンがなくなったらtrue
	bool EOT();
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);
	//マテリアル名に該当するマテリアルのアドレスを返す
	CMaterial* FindMaterial(char* name);
	//フレームの変換行列を更新
	void AnimateFrame();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//頂点にアニメーションを適用
	void AnimateVertex();
	//頂点計算を指定した合成行列で行う
	void AnimateVertex(CMatrix*);
	//mFrame配列を返す
	std::vector<CModelXFrame*>& GetFrames();
	//mAnimationSet配列を返す
	std::vector<CAnimationSet*>& GetAnimationSet();
	//mMaterial配列の取得
	std::vector<CMaterial*>& GetMaterial();

private:
	std::vector<CModelXFrame*> mFrame;       //フレームの配列
	std::vector<CAnimationSet*>mAnimationSet;//アニメーションセットの配列
	std::vector<CMaterial*>mMaterial;        //マテリアル配列
	bool IsDelimiter(char c);//cが区切り文字ならtrueを返す
	char* mpPointer;  //読み込み位置
	char mToken[1024];//取り出した単語の領域
};

//Frameクラス
class CModelXFrame {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
public:
	CModelXFrame(CModelX* model);
	~CModelXFrame();

	//描画
	void Render();
	//mIndexの値を返す
	int GetIndex();
	//合成行列の作成
	void SetAnimateCombined(CMatrix* parent);
	//mCombinedMatrix(合成行列)を返す
	const CMatrix& GetCombinedMatrix();
private:
	std::vector<CModelXFrame*> mChild;//子フレームの配列
	CMatrix mTransformMatrix;//変換行列
	CMatrix mCombinedMatrix; //合成行列
	char* mpName;//フレーム名前
	int mIndex;  //フレーム番号
	CMesh* mpMesh;//Meshデータ
};

//Meshクラス
class CMesh {
public:
	CMesh();
	~CMesh();

	//読み込み処理
	void Init(CModelX* model);
	//描画
	void Render();
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
	//頂点にアニメーションを適用
	void AnimateVertex(CModelX* model);
	//頂点計算を指定した合成行列で行う
	void AnimateVertex(CMatrix*);
private:
	int mVertexNum;    //頂点数
	CVector* mpVertex; //頂点データ
	int mFaceNum;      //面数
	int* mpVertexIndex;//面を構成する頂点インデックス
	int mNormalNum;    //法線数
	CVector* mpNormal; //法線ベクトル
	int mMaterialNum;     //マテリアル数
	int mMaterialIndexNum;//マテリアル番号数（面数）
	int* mpMaterialIndex; //マテリアル番号
	std::vector<CMaterial*> mMaterial;     //マテリアルデータ配列
	std::vector<CSkinWeights*>mSkinWeights;//スキンウェイト配列
	CVector* mpAnimateVertex;//アニメーション用頂点
	CVector* mpAnimateNormal;//アニメーション用法線
	//テクスチャ座標データ
	float* mpTextureCoords;
};

/*
CSkinWeights
スキンウェイトクラス
*/
class CSkinWeights {
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();

	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;//フレーム名
	int mFrameIndex;  //フレーム番号
	int mIndexNum;    //頂点番号数
	int* mpIndex;     //頂点番号配列
	float* mpWeight;  //頂点ウェイト配列
	CMatrix mOffset;  //オフセットマトリックス
};

/*
CAnimationSet
アニメーションセット
*/
class CAnimationSet {
	friend CModelX;
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();

	//時間の設定
	void SetTime(float time);
	//mTimeを返す
	float GetTime();
	//mMaxTimeを返す
	float GetMaxTime();
	//重みの設定
	void SetWeight(float weight);
	//アニメーション変換行列を計算する
	void AnimateMatrix(CModelX* model);
	//mAnimationを返す
	std::vector<CAnimation*>& GetAnimation();
private:
	float mTime;   //現在時間
	float mWeight; //重み
	float mMaxTime;//最大時間
	char* mpName;  //アニメーションセット名
	std::vector<CAnimation*> mAnimation;//アニメーション
};

/*
CAnimation
アニメーションクラス
*/
class CAnimation {
	friend CModelX;
	friend CAnimationSet;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;   //フレーム名
	int mFrameIndex;     //フレーム番号
	int mKeyNum;         //キー数（時間数）
	CAnimationKey* mpKey;//キーの配列
};

/*
CAnimationKey
アニメーションキー
*/
class CAnimationKey {
	friend CAnimation;
	friend CAnimationSet;
private:
	float mTime;    //時間
	CMatrix mMatrix;//行列
};
#endif

