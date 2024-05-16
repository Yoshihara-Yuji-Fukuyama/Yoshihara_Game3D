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
		//カメラの前方
		CVector cameraZ = CActionCamera::GetInstance()->GetVectorZ();
		//カメラの左方向
		CVector cameraX = CActionCamera::GetInstance()->GetVectorX();
		//キャラクタの前方
		CVector charZ = mMatrixRotate.GetVectorZ();
		//XZ平面にして正規化
		cameraZ.SetY(0.0f); cameraZ = cameraZ.Normalize();
		cameraX.SetY(0.0f); cameraX = cameraX.Normalize();
		charZ.SetY(0.0f); charZ = charZ.Normalize();
		//移動方向の設定
		CVector move;
		if (mInput.Key('A'))
		{
			move = move + cameraX;
		}
		if (mInput.Key('D'))
		{
			move = move - cameraX;
		}
		if (mInput.Key('W'))
		{
			move = move + cameraZ;
		}
		if (mInput.Key('S'))
		{
			move = move - cameraZ;
		}
		//移動あり
		if (move.Length() > 0.0f)
		{
			//遊び
			const float MARGIN = 0.06f;
			//正規化
			move = move.Normalize();
			//自分の向きと向かせたい向きで外積
			float cross = charZ.Cross(move).GetY();
			//自分の向きと向かせたい向きで内積
			float dot = charZ.Dot(move);
			//外積がプラスは左回転
			if (cross > MARGIN)
			{
				mRotation.SetY(mRotation.GetY() + 5.0f);
			}
			//外積がマイナスは右回転
			else if (cross < -MARGIN)
			{
				mRotation.SetY(mRotation.GetY() - 5.0f);
			}
			//前後の向きが同じとき内積は1.0f
			else if (dot < 1.0f - MARGIN)
			{
				mRotation.SetY(mRotation.GetY() - 5.0f);
			}
			//移動方向へ移動
			mPosition = mPosition + move * 0.1f;
			ChangeAnimation(1, true, 60);
		}
		else
		{
			ChangeAnimation(0, true, 60);
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
