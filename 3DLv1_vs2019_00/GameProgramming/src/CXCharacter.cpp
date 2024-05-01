#include "CXCharacter.h"

CXCharacter::CXCharacter()
{
	mScale = CVector(1.0f, 1.0f, 1.0f);
}

//更新処理
void CXCharacter::Update()
{
	//変換行列の更新
	CTransform::Update();
	//アニメーションを更新する
	Update(mMatrix);
}

//初期化処理
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//最初のアニメーションにする
	mAnimationIndex = 0;
	//繰り返し再生する
	mAnimationLoopFlg = true;
	//1アニメーション目の最大フレーム数
	mAnimationFrameSize = model->GetAnimationSet()[0]->GetMaxTime();
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->GetAnimationSet()[mAnimationIndex]->SetTime(mAnimationFrame);
	//アニメーションの重みを1.0（100%）にする
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(1.0f);
}

/*
アニメーションを切り替える
index:アニメーションの番号
loop:true　繰り返す
framesize:最後まで再生するのに使用されるフレーム数
*/
void CXCharacter::ChangeAnimation(int index, bool loop, float framesize)
{
	//同じ場合は切り替えない
	if (mAnimationIndex == index)return;
	//今のアニメーションの重みを0.0(0%)にする
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(0.0f);
	//番号、繰り返し、フレーム数を設定
	mAnimationIndex = index % mpModel->GetAnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//アニメーションのフレームを最初にする
	mAnimationFrame = 0.0f;
	mpModel->GetAnimationSet()[mAnimationIndex]->SetTime(mAnimationFrame);
	//アニメーションの重みを1.0(100%)にする
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(1.0f);
}

/*
更新する
matrix:移動、回転、拡大縮小の行列
*/
void CXCharacter::Update(CMatrix& matrix)
{
	//最後まで再生する
	if (mAnimationFrame <= mAnimationFrameSize)
	{
		//アニメーションの時間を計算
		mpModel->GetAnimationSet()[mAnimationIndex]->
			SetTime(mpModel->GetAnimationSet()[mAnimationIndex]->
			GetMaxTime() * mAnimationFrame / mAnimationFrameSize);
		//フレームを進める
		mAnimationFrame++;
	}
	else
	{
		//繰り返しの場合は、フレームを0に戻す
		if (mAnimationLoopFlg)
		{
			//アニメーションのフレームを最初にする
			mAnimationFrame = 0.0f;
			mpModel->GetAnimationSet()[mAnimationIndex]->
				SetTime(mAnimationFrame);
		}
		else
		{
			mpModel->GetAnimationSet()[mAnimationIndex]->
				SetTime(mpModel->GetAnimationSet()[mAnimationIndex]->
					GetMaxTime());
		}
	}
	//フレームの変換行列をアニメーションで更新する
	mpModel->AnimateFrame();
	//フレームの合成行列を計算する
	mpModel->GetFrames()[0]->SetAnimateCombined(&matrix);
	//頂点にアニメーションを適用する
	mpModel->AnimateVertex();
}

//描画する
void CXCharacter::Render()
{
	mpModel->Render();
}

//アニメーションの終了判定
bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

//アニメーション番号の取得
int CXCharacter::GetAnimationIndex()
{
	return mAnimationIndex;
}
