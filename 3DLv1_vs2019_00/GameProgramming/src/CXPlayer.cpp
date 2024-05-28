#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//回転速度
#define PLAYER_SPEED 0.1f//移動速度

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: IsLeftTurn(false)
	, IsRightTurn(false)
	/*/ : mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//頭,球コライダ
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//体,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//体,カプセルコライダ
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//剣,球コライダ
	*/
{
	//タグにプレイヤーを設定
	mCharaTag = ECharaTag::EPLAYER;
	spInstance = this;
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
}

//インスタンスの取得
CXPlayer* CXPlayer::GetInstance()
{
	return spInstance;
}

void CXPlayer::Update()
{
	//攻撃モーションのとき
	if (GetAnimationIndex() == 5)
	{
		//アニメーションが終了したら
		//待機アニメーションにする
		if (IsAnimationFinished() == true)
		{		
			ChangeAnimation(1, true, 90);
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
		//どの方向に移動しているか
		bool moveF = false;
		bool moveB = false;
		bool moveL = false;
		bool moveR = false;
		if (mInput.Key('W'))
		{
			move = move + cameraZ;
			moveF = true;
		}
		if (mInput.Key('S'))
		{
			move = move - cameraZ;
			moveB = true;
		}
		if (mInput.Key('A'))
		{
			move = move + cameraX;
			moveL = true;
		}
		if (mInput.Key('D'))
		{
			move = move - cameraX;
			moveR = true;
		}
		//移動あり
		if (move.Length() > 0.0f)
		{
			//遊び
			//const float MARGIN = 0.06f;
			//正規化
			move = move.Normalize();
			//自分の向きと向かせたい向きで外積
			//float cross = charZ.Cross(move).GetY();
			//自分の向きと向かせたい向きで内積
			//float dot = charZ.Dot(move);
			//外積がプラスは右回転
			/*if (cross > MARGIN)
			{
				mRotation.SetY(mRotation.GetY() - 5.0f);
			}
			//外積がマイナスは左回転
			else if (cross < -MARGIN)
			{
				mRotation.SetY(mRotation.GetY() + 5.0f);
			}
			//前後の向きが同じとき内積は1.0f
			else if (dot < 1.0f - MARGIN)
			{
				mRotation.SetY(mRotation.GetY() + 5.0f);
			}
			*/
			//移動方向へ移動
			mPosition = mPosition + move * PLAYER_SPEED;
			//ChangeAnimation(0, true, 90);
			if (moveF == true)
			{
				ChangeAnimation(0, true, 90);
			}
			else if (moveB == true)
			{
				ChangeAnimation(1, true, 90);
			}
			else if (moveL == true)
			{
				ChangeAnimation(2, true, 90);
			}
			else if (moveR == true)
			{
				ChangeAnimation(3, true, 90);
			}
		}
		//移動していなければ待機アニメーションに切り替え
		//正面を向く
		else
		{
			//両方がtrueだと正面近くを向いているので回転しない
			if (IsLeftTurn == false || IsRightTurn == false)
			{
				//遊び
				const float MARGIN = 0.06f;
				//自分の向きと向かせたい向きで外積
				float cross = charZ.Cross(cameraZ).GetY();
				//自分の向きと向かせたい向きで内積
				float dot = charZ.Dot(cameraZ);
				//外積がプラスは右回転
				if (cross > MARGIN)
				{
					mRotation.SetY(mRotation.GetY() - 5.0f);
					IsRightTurn = true;
				}
				//外積がマイナスは左回転
				else if (cross < -MARGIN)
				{
					mRotation.SetY(mRotation.GetY() + 5.0f);
					IsLeftTurn = true;
				}
				//前後の向きが同じとき内積は1.0f
				else if (dot < 1.0f - MARGIN)
				{
					mRotation.SetY(mRotation.GetY() + 5.0f);
				}
			}
			ChangeAnimation(4, true, 90);
		}
		//左クリックが押されたら、弾丸を飛ばす
		if (mInput.Key(VK_LBUTTON))
		{
			ChangeAnimation(5, true, 30);
		}
	}
	//変換行列、アニメーションの更新
	CXCharacter::Update();

	//カプセルコライダの更新
	//mColBody.Update();
}

//マウス移動でY軸回転
void CXPlayer::Turn(float turnHorizontal)
{
	//回転量の分だけ回転する
	mRotation = mRotation - CVector(0.0f, turnHorizontal, 0.0f);
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
	/*
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[12]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[9]);
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);*/
}
