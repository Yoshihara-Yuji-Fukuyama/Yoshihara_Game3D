#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//回転速度
#define PLAYER_VELOCITY CVector(0.0f,0.0f,0.1f)//移動速度

CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	//タグにプレイヤーを設定
	mCharaTag = ECharaTag::EPLAYER;
}

void CXPlayer::Update()
{
	//攻撃モーションのとき
	if (GetAnimationIndex() == 3)
	{
		//アニメーションが終了したら
		//攻撃終了アニメーションに変更
		if (IsAnimationFinished() == true)
		{		
			ChangeAnimation(4, false, 30);
		}
	}
	//攻撃終了モーションのとき
	else if (GetAnimationIndex() == 4)
	{
		//アニメーションが終了したら
		//待機アニメーションに変更
		if (IsAnimationFinished() == true)
		{	
			ChangeAnimation(0, true, 60);
		}
	}
	//上記以外のとき
	else
	{
		//Wキーが押されていれば、歩くモーションに変更
        //前方向に移動
		if (mInput.Key('W'))
		{
			ChangeAnimation(1, true, 60);
			mPosition = mPosition + PLAYER_VELOCITY * mMatrixRotate;
		}
		//Wキーが押されてないなら待機モーションに変更
		else
		{
			ChangeAnimation(0, true, 60);
		}
		//Aキーが押されたら左に回転
		if (mInput.Key('A'))
		{
			mRotation = mRotation + PLAYER_ROTATION;
		}
		//Dキーが押されたら右に回転
		else if (mInput.Key('D'))
		{
			mRotation = mRotation - PLAYER_ROTATION;
		}

		//左クリックが押されたら、攻撃モーションに変更
		if (mInput.Key(VK_LBUTTON))
		{
			ChangeAnimation(3, false, 30);
		}
	}

	//変換行列、アニメーションの更新
	CXCharacter::Update();
}

//初期設定
//球コライダの生成と、ボーンとの連動
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[12]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[9]);
	//剣
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
}
