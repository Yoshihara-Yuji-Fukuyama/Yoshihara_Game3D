#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//回転速度
#define PLAYER_SPEED 0.1f//移動速度

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//頭,球コライダ
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//体,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//体,カプセルコライダ
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//剣,球コライダ
{
	//タグにプレイヤーを設定
	mCharaTag = ECharaTag::EPLAYER;
	spInstance = this;
}

//インスタンスの取得
CXPlayer* CXPlayer::GetInstance()
{
	return spInstance;
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
		//移動方向の設定
		CVector move;
		if (mInput.Key('A'))
		{
			move.SetX(move.GetX() + 1.0f);
		}
		if (mInput.Key('D'))
		{
			move.SetX(move.GetX() - 1.0f);
		}
		if (mInput.Key('W'))
		{
			move.SetZ(move.GetZ() + 1.0f);
		}
		if (mInput.Key('S'))
		{
			move.SetZ(move.GetZ() - 1.0f);
		}
		//移動あり
		if (move.Length() > 0.0f)
		{
			move = move.Normalize();
			//移動方向へ移動
			mPosition = mPosition + move * PLAYER_SPEED;
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
	//カプセルコライダの更新
	mColBody.Update();
}

//衝突判定
void CXPlayer::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
		//自分のコライダがカプセルの場合
	case CCollider::EType::ECAPSULE:
		//相手のコライダもカプセルなら
		if (o->GetType() == CCollider::EType::ECAPSULE)
		{
			CVector adjust;//調整用
			//カプセル同士が衝突しているなら
			if (CCollider::CollisionCapsuleCapseule(m, o, &adjust))
			{
				//衝突しない位置に戻す
				mPosition = mPosition + adjust;
				//行列更新
				CTransform::Update();
			}
		}
		break;
	}
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
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
}
