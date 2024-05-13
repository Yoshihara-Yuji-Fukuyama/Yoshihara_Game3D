#include <stdio.h>
#include <string.h>
#include "CModelX.h"
#include "glut.h"
#include <ctype.h> //isspace関数の宣言
#include "CMaterial.h"//CMaterialを使えるようにするため

CModelX::CModelX()
	: mpPointer(nullptr)
	, mLoaded(false)
{
	//mTokenを初期化
	memset(mToken, 0, sizeof(mToken));
}

CModelX::~CModelX()
{
	//フレームの解放
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
	//アニメーションセットの解放
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
	}
	//マテリアルの解放
	for (size_t i = 0; i < mMaterial.size(); i++)
	{
		delete mMaterial[i];
	}
}
//文字列データから、単語を1つ取得する
char* CModelX::GetToken()
{
	char* p = mpPointer;
	char* q = mToken;
	//タブ(\t)空白()改行(\r)(\n),;"の区切り文字以外になるまで読み飛ばす
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}')
	{
		//{または}ならmTokenに代入し次の文字へ
		*q++ = *p++;
	}
	else
	{
		//タブ(\t)空白()改行(\r)(\n),;"の区切り文字、
		//または、}の文字になるまでmTokenに代入する
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}

	*q = '\0'; //mTokenの最後に\0を代入
	mpPointer = p; //次の読み込むポイントを更新する

	//もしmTokenが//の場合は、コメントなので改行まで読み飛ばす
	/*
	strcmp(文字列1,文字列2)
	文字列1と文字列2が等しい場合、0を返します。
	文字列1と文字列2が等しくない場合、0以外を返します
	*/

	if (!strcmp("//", mToken))
	{
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置の更新
		mpPointer = p;
		//単語を取得する(再帰呼び出し)
		return GetToken();
	}
	return mToken;
}
//mTokenのポインタを返す
char* CModelX::GetOnlyToken()
{
	return mToken;
}


void CModelX::Load(char* file)
{
	//ファイルサイズを取得する

	FILE* fp; //ファイルポインタ変数の作成

	fp = fopen(file, "rb"); //ファイルをオープンする
	if (fp == NULL)
	{
		//エラーチェック
		printf("fopen error:%s\n", file);
		return;
	}

	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ＋1バイト分の領域を確保
	char* buf = mpPointer = new char[size + 1];
	
	//ファイルから3Dモデルのデータを読み込む

	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';
	fclose(fp); //ファイルをクローズする

	//ダミールートフレームの作成
	CModelXFrame* p = new CModelXFrame();
	//名前なし
	p->mpName = new char[1];
	p->mpName[0] = '\0';
	//フレーム配列に追加
	mFrame.push_back(p);

	//文字列の最後まで繰り返し
	while (*mpPointer != '\0')
	{
		GetToken(); //単語の取得
		//template 読み飛ばし
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//Materialの時
		else if (strcmp(mToken, "Material") == 0)
		{
			new CMaterial(this);
		}
		//単語がFrameの場合
		else if (strcmp(mToken, "Frame") == 0)
		{
			//フレーム名取得
			GetToken();
			if (strchr(mToken, '{'))
			{
				//フレーム名なし　スキップ
				SkipNode();
				GetToken(); // }
			}
			else
			{
				//フレームがなければ
				if (FindFrame(mToken) == 0)
				{
					//フレームを作成する
					p->mChild.push_back(new CModelXFrame(this));
				}
			}
		}
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);
		}
	}

	SAFE_DELETE_ARRAY(buf); //確保した領域を開放する
	//スキンウェイトのフレーム番号設定
	SetSkinWeightFrameIndex();

	mLoaded = true;//読み込み済み
}
//読み込み済みか判定
bool CModelX::IsLoaded()
{
	return mLoaded;
}
//ノードを読み飛ばす
void CModelX::SkipNode()
{
	//文字が終わったら終了
	while (*mpPointer != '\0')
	{
		GetToken();//次の単語取得
		//{が見つかったらループ終了
		if (strchr(mToken, '{')) break;
	}
	int count = 1;
	//文字が終わるか、カウントが0になったら終了
	while (*mpPointer != '\0' && count > 0)
	{
		GetToken();//次の単語取得
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{')) count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}')) count--;
	}
}
//すべてのフレームの描画処理を呼び出す
void CModelX::Render()
{
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		mFrame[i]->Render();
	}
}
//トークンがなくなったらtrue
bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
FindFrame(フレーム名)
フレーム名に該当するフレームのアドレスを返す
*/
CModelXFrame* CModelX::FindFrame(char* name)
{
	//イテレータの作成
	std::vector<CModelXFrame*>::iterator itr;
	//先頭から最後まで繰り返す
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++)
	{
		//名前が一致したか？
		if (strcmp(name, (*itr)->mpName) == 0)
		{
			//一致したらそのアドレスを返す
			return *itr;
		}
	}
	//一致するフレームがない場合はnullptrを返す
	return nullptr;
}

/*
FindMaterial(マテリアル名)
マテリアル名に外とるするマテリアルのアドレスを返す*/
CMaterial* CModelX::FindMaterial(char* name)
{
	//マテリアル配列のイテレータ作成
	std::vector<CMaterial*>::iterator itr;
	//マテリアル配列を先頭から順に検索
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++)
	{
		//名前が一致すればマテリアルのポインタを返却
		if (strcmp(name, (*itr)->Name()) == 0)
		{
			return *itr;
		}
	}
	//ないときはnullptrを返却
	return nullptr;
}

/*
AnimateFrame
フレームの変換行列をアニメーションデータで更新する
*/
void CModelX::AnimateFrame()
{
	//アニメーションで適用されるフレームの
	//変換行列をゼロクリアする
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//重みが0は飛ばす
		if (animSet->mWeight == 0)continue;

		//フレーム分(Animation分)繰り返す
		for (size_t j = 0; j < animSet->GetAnimation().size(); j++)
		{
			CAnimation* animation = animSet->GetAnimation()[j];
			//該当するフレームの変換行列をゼロクリアする
			memset(
				&mFrame[animation->mFrameIndex]
				->mTransformMatrix,
				0, sizeof(CMatrix));
		}
	}
	//アニメーションに該当するフレームの変換行列を
	//アニメーションのデータで設定する
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//重みが0は飛ばす
		if (animSet->mWeight == 0)continue;
		animSet->AnimateMatrix(this);
	}

#ifdef _DEBUG
	//全フレーム名とmTransformMatrixの設定を表示
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		printf("Frame:%s\n", mFrame[i]->mpName);
		for (int j = 0; j < 4; j++)
		{
			printf("%10f %10f %10f %10f\n",
				mFrame[i]->mTransformMatrix.GetM(j, 0),
				mFrame[i]->mTransformMatrix.GetM(j, 1),
				mFrame[i]->mTransformMatrix.GetM(j, 2),
				mFrame[i]->mTransformMatrix.GetM(j, 3));
		}
	}
#endif
}

//スキンウェイトのフレーム番号設定
void CModelX::SetSkinWeightFrameIndex()
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュがあれば
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->SetSkinWeightFrameIndex(this);
		}
	}
}

/*
AnimateVertex
頂点にアニメーションを適用する
*/
void CModelX::AnimateVertex()
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュに面があれば
		if (mFrame[i]->mpMesh != nullptr)
		{
			//頂点をアニメーションで更新する
			mFrame[i]->mpMesh->AnimateVertex(this);
		}
	}
}

//頂点計算を指定した合成行列で行う
void CModelX::AnimateVertex(CMatrix* mat)
{
	//フレーム数分繰り返し
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//メッシュがあれば
		if (mFrame[i]->mpMesh)
		{
			//頂点をアニメーションで更新する
			mFrame[i]->mpMesh->AnimateVertex(mat);
		}
	}
}

//アニメーションを抜き出す
void CModelX::SeparateAnimationSet(int idx, int start, int end, char* name)
{
	CAnimationSet* anim = mAnimationSet[idx];//分割するアニメーションセットを確定
	CAnimationSet* as = new CAnimationSet(); //アニメーションセットの生成
	as->mpName = new char[strlen(name) + 1]; //名前用の領域を確保
	strcpy(as->mpName, name);  //名前をコピー
	as->mMaxTime = end - start;//再生時間を計算
	//既存のアニメーション分繰り返し
	for (size_t i = 0; i < anim->mAnimation.size(); i++)
	{
		CAnimation* animation = new CAnimation();//アニメーションの生成
		animation->mpFrameName = 
			new char[strlen(anim->mAnimation[i]->mpFrameName) + 1];//フレーム名用の領域を確保
		strcpy(animation->mpFrameName, anim->mAnimation[i]->mpFrameName);//フレーム名のコピー
		animation->mFrameIndex = anim->mAnimation[i]->mFrameIndex;//フレーム番号を代入
		animation->mKeyNum = end - start + 1;//時間数
		animation->mpKey = new CAnimationKey[animation->mKeyNum];//アニメーションキーの生成
		animation->mKeyNum = 0;//時間数をゼロにする
		//jが終了時間以下かつ時間数未満の間繰り返す
		//アニメーションキーコピー
		for (int j = start; j <= end && j < anim->mAnimation[i]->mKeyNum; j++)
		{
			if (j < anim->mAnimation[i]->mKeyNum)
			{
				animation->mpKey[animation->mKeyNum] = anim->mAnimation[i]->mpKey[j];
			}
			else
			{
				animation->mpKey[animation->mKeyNum] =
					anim->mAnimation[i]->mpKey[anim->mAnimation[i]->mKeyNum - 1];
			}
			animation->mpKey[animation->mKeyNum].mTime = animation->mKeyNum++;
		}
		as->mAnimation.push_back(animation);//アニメーションの追加
	}
	mAnimationSet.push_back(as);//アニメーションセットの追加
}
//アニメーションセットの追加
void CModelX::AddAnimationSet(const char* file)
{
	//ファイルサイズを取得
	FILE* fp;//ファイルポインタ変数の作成
	fp = fopen(file, "rb");//ファイルをオープンする
	if (fp == NULL)//エラーチェック
	{
		printf("fopen error:%s\n", file);
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);

	//ファイルサイズ＋1バイト文の領域を確保
	char* buf = mpPointer = new char[size + 1];
	/*ファイルから3Dモデルのデータを読み込む*/
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';
	fclose(fp);//ファイルをクローズする

	//文字列の最後まで繰り返し
	while (*mpPointer != '\0')
	{
		GetToken();//単語の取得
		//template 読み飛ばし
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//単語がAnimationSetの場合
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);//アニメーションセットの生成
		}
	}
	SAFE_DELETE_ARRAY(buf);//確保した領域を解放
}

//mFrame配列を返す
std::vector<CModelXFrame*>& CModelX::GetFrames()
{
	return mFrame;

}
//mAnimationSet配列を返す
std::vector<CAnimationSet*>& CModelX::GetAnimationSet()
{
	return mAnimationSet;
}
//mMaterial配列を返す
std::vector<CMaterial*>& CModelX::GetMaterial()
{
	return mMaterial;
}

//cが区切り文字ならtrueを返す
bool CModelX::IsDelimiter(char c)
{
	//cが0より小さい時、falseを返す
	if (c < 0)
	{
		return false;
	}
	//isspace(c)
	//cが空白文字なら0以外を返す
	if (isspace(c) != 0)
		return true;
	/*
	strchr(文字列,文字)
	文字列に文字が含まれていれば
	見つかった文字へのポインタを返す
	見つからなかったらNULLを返す
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//区切り文字ではない
	return false;
}



CModelXFrame::CModelXFrame()
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{
}

/*
model:CModelXインスタンスへのポインタ
フレームを作成する
読み込み中にFrameが見つかれば、フレームを作成し、
子フレームに追加する*/
CModelXFrame::CModelXFrame(CModelX* model)
	: mpName(nullptr)
	, mIndex(0)
	, mpMesh(nullptr)
{
	//現在のフレーム配列の要素数を取得し設定する
	mIndex = model->mFrame.size();
	//CModelXのフレーム配列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位行列にする
	mTransformMatrix.SetIdentity();
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名をコピーする
	strcpy(mpName, model->mToken);
	//次の単語（｛の予定）を取得する
	model->GetToken();//{
	//文字がなくなったら終わり
	while (*model->mpPointer != '\0')
	{
		//次の単語取得
		model->GetToken();//Frame
		//}かっこの場合は終了
		if (strchr(model->mToken, '}')) break;
		//新たなフレームの場合は、子フレームに追加
		if (strcmp(model->mToken, "Frame") == 0)
		{
			//フレーム名取得
			model->GetToken();
			if (strchr(model->mToken, '{'))
			{
				//フレーム名なし スキップ
				model->SkipNode();
				model->GetToken(); // }
			}
			else
			{
				//フレームがなければ
				if (model->FindFrame(model->mToken) == 0)
				{
					//フレームを作成し、子フレームの配列に追加
					mChild.push_back(new CModelXFrame(model));
				}
			}
		}
		//FrameTransformMatrixの場合、行列に追加する
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0)
		{
			model->GetToken();// {
			for (int i = 0; i < mTransformMatrix.GetSize(); i++)
			{
				mTransformMatrix.GetM()[i] = atof(model->GetToken());
			}
			model->GetToken();// }
		}
		//Meshの場合、CMeshを作成
		else if (strcmp(model->mToken, "Mesh") == 0)
		{
			mpMesh = new CMesh();
			mpMesh->Init(model);
		}
		else
		{
			//上記以外の要素は読み飛ばす
			model->SkipNode();
		}
	}
	//デバッグバージョンのみ有効
    #ifdef _DEBUG
	    printf("%s\n", mpName);
		mTransformMatrix.Print();
    #endif
}

CModelXFrame::~CModelXFrame()
{
	//子フレームをすべて開放する
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++)
	{
		delete* itr;
	}
	//名前のエリア解放する
	SAFE_DELETE_ARRAY(mpName);

	//mpMeshがnullptr以外ならインスタンスを削除
	if (mpMesh != nullptr)
	{
		delete mpMesh;
	}
}

/*
Render
メッシュが存在すれば描画する
*/
void CModelXFrame::Render()
{
	if (mpMesh != nullptr)
	{
		mpMesh->Render();
	}
}

int CModelXFrame::GetIndex()
{
	return mIndex;
}

/*
AnimateCombined
合成行列の作成
*/
void CModelXFrame::SetAnimateCombined(CMatrix* parent)
{
	//自分の変換行列に、親からの変換行列を掛ける
	mCombinedMatrix = mTransformMatrix * (*parent);
	//子フレームの合成行列を作成する
	for (size_t i = 0; i < mChild.size(); i++)
	{
		mChild[i]->SetAnimateCombined(&mCombinedMatrix);
	}

#ifdef _DEBUG
	printf("Frame:%s\n", mpName);
	for (int i = 0; i < 4; i++)
	{
		printf("%10f % 10f % 10f % 10f\n",
			mCombinedMatrix.GetM(i, 0),
			mCombinedMatrix.GetM(i, 1),
			mCombinedMatrix.GetM(i, 2), 
			mCombinedMatrix.GetM(i, 3));
	}
#endif
}

const CMatrix& CModelXFrame::GetCombinedMatrix()
{
	return mCombinedMatrix;
}


//CMeshのコンストラクタ
CMesh::CMesh()
	: mVertexNum(0)
	, mpVertex(nullptr)
	, mFaceNum(0)
	, mpVertexIndex(nullptr)
	, mNormalNum(0)
	, mpNormal(nullptr)
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
	, mpAnimateVertex(nullptr)
	, mpAnimateNormal(nullptr)
	, mpTextureCoords(nullptr)
{

}

//CMeshのデストラクタ
CMesh::~CMesh()
{
	SAFE_DELETE_ARRAY(mpVertex);
	SAFE_DELETE_ARRAY(mpVertexIndex);
	SAFE_DELETE_ARRAY(mpNormal);
	SAFE_DELETE_ARRAY(mpMaterialIndex);
	//スキンウェイトの削除
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
	SAFE_DELETE_ARRAY(mpAnimateVertex);
	SAFE_DELETE_ARRAY(mpAnimateNormal);
	SAFE_DELETE_ARRAY(mpTextureCoords);
}

/*
Init
Meshのデータを取り込む
*/
void CMesh::Init(CModelX* model)
{
	model->GetToken();// { or 名前
	if (!strchr(model->GetOnlyToken(), '{'))
	{
		//名前の場合、次が{
		model->GetToken();// {
	}

	//頂点数の取得
	mVertexNum = atoi(model-> GetToken());
	//頂点数分エリア確保
	mpVertex = new CVector[mVertexNum];
	mpAnimateVertex = new CVector[mVertexNum];
	//頂点数分データを取り込む
	for (int i = 0; i < mVertexNum; i++)
	{
		mpVertex[i].SetX(atof(model->GetToken()));
		mpVertex[i].SetY(atof(model->GetToken()));
		mpVertex[i].SetZ(atof(model->GetToken()));
	}

	//面数読み込み
	mFaceNum = atoi(model->GetToken());
	//頂点数は1面に3頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		model->GetToken(); //頂点数読み飛ばし
		mpVertexIndex[i] = atoi(model->GetToken());
		mpVertexIndex[i + 1] = atoi(model->GetToken());
		mpVertexIndex[i + 2] = atoi(model->GetToken());
	}
	
	//単語がある間繰り返し
	while (!model->EOT())
	{
		model->GetToken(); //MeshNormals
		// } かっこの場合は終了
		if (strchr(model->GetOnlyToken(), '}'))
			break;

		//MeshNormalsの時
		if (strcmp(model->GetOnlyToken(), "MeshNormals") == 0)
		{
			model->GetToken();// {
			//法線データ数を取得
			mNormalNum = atoi(model->GetToken());
			//法線のデータを配列に取り込む
			CVector* pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++)
			{
				pNormal[i].SetX(atof(model->GetToken()));
				pNormal[i].SetY(atof(model->GetToken()));
				pNormal[i].SetZ(atof(model->GetToken()));
			}
			//法線数＝面数×3
			mNormalNum = atoi(model->GetToken()) * 3;//FaceNum
			int ni;
			//頂点ごとに法線データを設定する
			mpNormal = new CVector[mNormalNum];
			mpAnimateNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i += 3)
			{
				model->GetToken(); // 3
				ni = atoi(model->GetToken());
				mpNormal[i] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 1] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 2] = pNormal[ni];
			}
			delete[] pNormal;
			model->GetToken(); // }
		}//End of MeshNormals

		//MeshMaterialListのとき
		else if (strcmp(model->GetOnlyToken(), "MeshMaterialList") == 0)
		{
			model->GetToken(); //{
			//Materialの数
			mMaterialNum = atoi(model->GetToken());
			//FaceNum
			mMaterialIndexNum = atoi(model->GetToken());
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++)
			{
				mpMaterialIndex[i] = atoi(model->GetToken());
			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++)
			{
				model->GetToken(); //Material
				if (strcmp(model->GetOnlyToken(), "Material") == 0)
				{
					mMaterial.push_back(new CMaterial(model));
				}
				else
				{
					// { 既出
					model->GetToken(); //MaterialName
					mMaterial.push_back(
						model->FindMaterial(model->GetOnlyToken()));
					model->GetToken(); // }
				}
			}
			model->GetToken(); // }
		}//End of MeshMaterialList

		//SkinWeightsのとき
		else if (strcmp(model->GetOnlyToken(), "SkinWeights") == 0)
		{
			//CSkinWeightsクラスのインスタンスを作成し、配列に追加
			mSkinWeights.push_back(new CSkinWeights(model));
		}//End of SkinWeights

		//テクスチャ座標の時
		else if (strcmp(model->GetOnlyToken(), "MeshTextureCoords") == 0)
		{
			model->GetToken(); // {
			//テクスチャ座標数を取得
			int textureCoordsNum = atoi(model->GetToken()) * 2;
			//テクスチャ座標のデータを配列に取り込む
			mpTextureCoords = new float[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++)
			{
				mpTextureCoords[i] = atof(model->GetToken());
			}
			model->GetToken(); // }
		}//End of MeshTextureCoords
		
		else
		{
			//以外のノードは読み飛ばし
			model->SkipNode();
		}
	}


	//デバッグバージョンのみ有効
    #ifdef _DEBUG
	//頂点数
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
		printf("%10f %10f %10f\n", 
			mpVertex[i].GetX(), mpVertex[i].GetY(), mpVertex[i].GetZ());
	}
	//面数
	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		printf("%3d %3d %3d\n", 
			mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	}
	//法線数
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++)
	{
		printf("%12f %12f %12f\n", 
			mpNormal[i].GetX(), mpNormal[i].GetY(), mpNormal[i].GetZ());
	}

    #endif
}

/*
Render
画面に描画する
*/
void CMesh::Render()
{
	/* 頂点データ、法線データの配列を有効にする */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//テクスチャマッピングの配列を有効にする
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* 頂点データ、法線データ,テクスチャ座標の場所を指定する */
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);

	/* 頂点のインデックスの場所を指定して図形を描画する */
	for (int i = 0; i < mFaceNum; i++)
	{
		//マテリアルを適用する
		mMaterial[mpMaterialIndex[i]]->Enabled();
		glDrawElements(GL_TRIANGLES, 3,
			GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
		mMaterial[mpMaterialIndex[i]]->Disabled();
	}

	/* 頂点データ、法線データの配列を無効にする */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

//スキンウェイトにフレーム番号を設定
void CMesh::SetSkinWeightFrameIndex(CModelX* model)
{
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム名のフレームを取得する
		CModelXFrame* frame = model->FindFrame(mSkinWeights[i]->mpFrameName);
		//フレーム番号を設定する
		mSkinWeights[i]->mFrameIndex = frame->GetIndex();
	}
}

//頂点にアニメーションを適用
void CMesh::AnimateVertex(CModelX* model)
{
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//オフセット行列とフレーム合成行列を合成
		CMatrix mSkinningMatrix = 
			mSkinWeights[i]->mOffset * model->GetFrames()[frameIndex]->GetCombinedMatrix();
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//法線を正規化する
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}

void CMesh::AnimateVertex(CMatrix* mat)
{
	//アニメーション用の頂点エリアクリア
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//スキンウェイト分繰り返し
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//フレーム番号取得
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//フレーム合成行列にオフセット行列を合成
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * mat[frameIndex];
		//頂点数分繰り返し
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//頂点番号取得
			int index = mSkinWeights[i]->mpIndex[j];
			//重み取得
			float weight = mSkinWeights[i]->mpWeight[j];
			//頂点と法線を更新する
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}

	}
	//法線を正規化
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}



/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken(); // {
	model->GetToken(); //FrameName
	//フレーム名確保、設定
	mpFrameName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpFrameName, model->GetOnlyToken());

	//頂点番号数取得
	mIndexNum = atoi(model->GetToken());
	//頂点番号数が0を超える
	if (mIndexNum > 0)
	{
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];
		mpWeight = new  float[mIndexNum];
		//頂点番号取得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//頂点ウェイト取得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//オフセット行列
	for (int i = 0; i < 16; i++)
	{
		mOffset.GetM()[i] = atof(model->GetToken());
	}
	model->GetToken(); // }

	//デバッグバージョンのみ有効
    #ifdef _DEBUG
	printf("SkinWeights %s\n", mpFrameName);
	for (int i = 0; i < mIndexNum; i++)
	{
		printf("%d %f\n",
			mpIndex[i], mpWeight[i]);
	}
	for (int i = 0; i < 16; i += 4)
	{
		printf("%9f %9f %9f %9f\n",
			mOffset.GetM()[i], mOffset.GetM()[i + 1], mOffset.GetM()[i + 2], mOffset.GetM()[i + 3]);
	}
    #endif
}

//CSkinWeightsのデストラクタ
CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpIndex);
	SAFE_DELETE_ARRAY(mpWeight);
}

//CAnimationSetのデフォルトコンストラクタ
CAnimationSet::CAnimationSet()
	: mpName(nullptr)
	, mTime(0)
	, mWeight(0)
	, mMaxTime(0)
{
}
//CAnimationSetのコンストラクタ
CAnimationSet::CAnimationSet(CModelX* model)
	: mpName(nullptr)
	, mTime(0)
	, mWeight(0)
	, mMaxTime(0)
{
	model->mAnimationSet.push_back(this);
	model->GetToken(); //Animation Name
	//アニメーションセット名を退避
	mpName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpName, model->GetOnlyToken());
	model->GetToken(); // {
	while (!model->EOT())
	{
		model->GetToken(); // } or Animation
		if (strchr(model->GetOnlyToken(), '}'))break;
		if (strcmp(model->GetOnlyToken(), "Animation") == 0)
		{
			//Animation要素読み込み
			mAnimation.push_back(new CAnimation(model));
		}
	}

	//終了時間
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;

	//デバッグバージョンのみ有効
    #ifdef _DEBUG
	printf("AnimationSet:%s\n", mpName);
    #endif
}
//CAnimationSetのデストラクタ
CAnimationSet::~CAnimationSet()
{
	SAFE_DELETE_ARRAY(mpName);
	//アニメーション要素の削除
	for (size_t i = 0; i < mAnimation.size(); i++)
	{
		delete mAnimation[i];
	}
}
//時間の設定
void CAnimationSet::SetTime(float time)
{
	mTime = time;
}
//mTimeを返す
float CAnimationSet::GetTime()
{
	return mTime;
}
//mMaxTimeを返す
float CAnimationSet::GetMaxTime()
{
	return mMaxTime;
}
//重みの設定
void CAnimationSet::SetWeight(float weight)
{
	mWeight = weight;
}
//アニメーション変換行列を計算する
void CAnimationSet::AnimateMatrix(CModelX* model)
{
	//重みが0は飛ばす
	if (mWeight == 0)return;
	//フレーム分(Animation分)繰り返す
	for (size_t j = 0; j < mAnimation.size(); j++)
	{
		//フレームを取得する
		CAnimation* animation = mAnimation[j];
		//キーがない場合発議のアニメーションへ
		if (animation->mpKey == nullptr)continue;
		//該当するフレームの取得
		CModelXFrame* frame = model->mFrame[animation->mFrameIndex];
		//最初の時間より小さい場合
		if (mTime < animation->mpKey[0].mTime)
		{
			//変換行列を0コマ目の行列で更新
			frame->mTransformMatrix += animation->mpKey[0].mMatrix * mWeight;
		}
		//最後の時間より大きい場合
		else if (mTime >= animation->mpKey[animation->mKeyNum - 1].mTime)
		{
			//変換行列を最後のコマの行列で更新
			frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * mWeight;
		}
		else
		{
			//時間の途中の場合
			for (int k = 1; k < animation->mKeyNum; k++)
			{
				//変換行列を、線形補間にて更新
				if (mTime < animation->mpKey[k].mTime &&
					animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime)
				{
					float r = (animation->mpKey[k].mTime - mTime) /
						(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);

					frame->mTransformMatrix += (animation->mpKey[k - 1].mMatrix * r +
						animation->mpKey[k].mMatrix * (1 - r)) * mWeight;
					break;
				}
			}
		}
	}
}
//mAnimationを返す
std::vector<CAnimation*>& CAnimationSet::GetAnimation()
{
	return mAnimation;
}

//CAnimationのデフォルトコンストラクタ
CAnimation::CAnimation()
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
}
//CAnimationのコンストラクタ
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->GetOnlyToken(), '{'))
	{
		model->GetToken(); // {
	}
	else
	{
		model->GetToken(); // {
		model->GetToken(); // {
	}

	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpFrameName, model->GetOnlyToken());
	mFrameIndex =
		model->FindFrame(model->GetOnlyToken())->GetIndex();
	model->GetToken(); // }

	//キーの配列を保存しておく配列
	CMatrix* key[4] = { nullptr,nullptr,nullptr,nullptr };
	//時間の配列を保存しておく配列
	float* time[4] = { nullptr,nullptr,nullptr,nullptr };

	while (!model->EOT())
	{
		model->GetToken();// } or AnimationKey
		if (strchr(model->GetOnlyToken(), '}'))break;
		if (strcmp(model->GetOnlyToken(), "AnimationKey") == 0)
		{
			model->GetToken(); // {
			//データのタイプ取得
			int type = atoi(model->GetToken());
			//時間数取得
			mKeyNum = atoi(model->GetToken());

			switch (type)
			{
			case 0: //Rotation Quaternion
				//行列の配列を時間数分確保
				key[type] = new CMatrix[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数文繰り返す
				for (int i = 0; i < mKeyNum; i++)
				{
					//時間取得
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //4を読み飛ばし
					//w,x,y,zを取得
					float w = atof(model->GetToken());
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//クォータニオンから回転行列に変換
					key[type][i].Quaternion(x, y, z, w);
				}
				break;
			case 1: //拡大・縮小の行列作成
				//行列の配列を時間数分確保
				key[type] = new CMatrix[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数文繰り返す
				for (int i = 0; i < mKeyNum; i++)
				{
					//時間取得
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //3を読み飛ばし
					//x,y,zを取得
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//拡大・縮小行列に変換
					key[type][i].SetScale(x, y, z);
				}
				break;
			case 2: //移動の行列作成
				//行列の配列を時間数分確保
				key[type] = new CMatrix[mKeyNum];
				//時間の配列を時間数分確保
				time[type] = new float[mKeyNum];
				//時間数文繰り返す
				for (int i = 0; i < mKeyNum; i++)
				{
					//時間取得
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //3を読み飛ばし
					//x,y,zを取得
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//移動行列に変換
					key[type][i].SetTranlate(x, y, z);
				}
				break;
			case 4: //行列データを取得
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					mpKey[i].mTime = atof(model->GetToken());//Time
					model->GetToken(); //16
					for (int j = 0; j < 16; j++)
					{
						mpKey[i].mMatrix.GetM()[j] = atof(model->GetToken());
					}
				}
				break;
			}
			model->GetToken(); // }
		}
		else 
		{
			model->SkipNode();
		}
	} //whileの終わり

	//行列データではないとき
	if (mpKey == nullptr)
	{
		//時間数分キーを作成
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++)
		{
			//時間設定
			mpKey[i].mTime = time[2][i];//Time
			//行列作成 Scale * Rotation * Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
		}
	}
	//確保したエリア開放
	for (int i = 0; i < ARRAY_SIZE(key); i++)
	{
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}

	//デバッグバージョンのみ有効
#ifdef _DEBUG
	printf("Animation:%s\n", mpFrameName);
	for (int i = 0; i < 16; i += 4)
	{
		printf("%9f %9f %9f %9f\n",
			mpKey[1].mMatrix.GetM()[i], mpKey[1].mMatrix.GetM()[i + 1], mpKey[1].mMatrix.GetM()[i + 2], mpKey[1].mMatrix.GetM()[i + 3]);
	}
#endif
}
//CAnimationのデストラクタ
CAnimation::~CAnimation()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpKey);
}