#include "CXEnemy.h"
#include "CXPlayer.h"

#define GRAVITY_AND_JUMPDEF 0.1f//重力とジャンプの基準値

CXEnemy::CXEnemy()
	: mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f, CCollider::ETag::EHEAD)//頭,球コライダ
	, mColSphereLeg(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f, CCollider::ETag::ELEG)//足,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//体,カプセルコライダ
	, mColSphereSearch(this, nullptr, CVector(), 30.0f, CCollider::ETag::ESEARCH)//索敵用、球コライダ
	, mEnemySpeed(0.1f)
	, IsMove(false)
{
	//タグに敵を設定
	mCharaTag = ECharaTag::EENEMY;
	//サイズ設定
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//前を向ける
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
}


//更新
void CXEnemy::Update()
{
	//キャラクタの前方
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ平面にして正規化
	charZ.SetY(0.0f); charZ = charZ.Normalize();

	//上昇中じゃないかつ地面に接触していないかつY座標が-1以上なら重力適用
	if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() >= -1.0f)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	//ジャンプ上昇アニメーションの時
	if (GetAnimationIndex() == 7)
	{
		mPosition.SetY(mPosition.GetY() + GRAVITY_AND_JUMPDEF);
		//アニメーションが終了したら
		//降下アニメーションにする
		if (IsAnimationFinished() == true)
		{
			ChangeAnimation(8, false, 90 * mJumpPower);
		}
	}
	//ジャンプ降下アニメーションの時
	else if (GetAnimationIndex() == 8)
	{
		//アニメーションが終了したら
		//待機アニメーションにする
		if (IsAnimationFinished() == true)
		{
			ChangeAnimation(1, true, 90);
			IsJump = false;
		}
	}
	//リロードアニメーションの時
	if (GetAnimationIndex() == 10 || GetAnimationIndex() == 11)
	{
		//動きながらのリロードなら
		if (GetAnimationIndex() == 11)
		{
			mPosition = mPosition - charZ * mEnemySpeed;
		}
		//アニメーションが終了したら
		//待機アニメーションにする
		if (IsAnimationFinished() == true)
		{
			ChangeAnimation(1, true, 90);
			IsReloading = false;
			IsWalkReload = false;
			IsWaitReload = false;
		}
	}
	//プレイヤーを見つけていたら
	if (IsFoundPlayer == true)
	{
		//プレイヤーの方へ進む
		//プレイヤーへの方向を計算
		CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
		//プレイヤーとの距離が10以上の場合近づく
		if (moveDirection.Length() >= 10.0f)
		{
			moveDirection = moveDirection.Normalize();
			ChangeAnimation(0, true, 90 * (1 - (mEnemySpeed * 0.1f)));
			IsMove = true;
		}
		//プレイヤーとの距離が10未満の場合待機アニメーションにする
		else
		{
			moveDirection = moveDirection.Normalize();
			ChangeAnimation(4, true, 90);
		}
		//trueの間プレイヤーの方向へ移動する
		if (IsMove == true)
		{
			mPosition = mPosition - moveDirection * 0.1f;
			if (IsAnimationFinished() == true)
			{
				IsMove = false;
			}
		}
		//TODO:敵の移動
		//移動方向を向かせる
		ChangeDirection(charZ, moveDirection * -1);

	}
	//変換行列、アニメーションの更新
	CXCharacter::Update();
	//カプセルコライダの更新
	mColBody.Update();
}

//武器の更新
void CXEnemy::WeponUpdate()
{
	mWepon.Update();
}
//武器の描画
void CXEnemy::WeponRender()
{
	mWepon.Render();
}


//衝突処理
void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
		//自分のコライダがカプセルの場合
		//衝突したら重ならない位置に戻す
	case CCollider::EType::ECAPSULE:
		//相手のコライダもカプセルなら
		if (o->GetType() == CCollider::EType::ECAPSULE)
		{
			CVector adjust;//調整用
			//カプセル同士が衝突しているなら
			if (CCollider::CollisionCapsuleCapseule(m, o, &adjust))
			{
				//重ならない位置に戻す
				mPosition = mPosition + adjust;
				//行列更新
				CTransform::Update();
			}
		}
		break;

		//自分のコライダが球の場合
		//敵の攻撃と衝突したらダウンする
	case CCollider::EType::ESPHERE:
		//自分のコライダのタグが頭
		if (m->GetTag() == CCollider::ETag::EHEAD)
		{
			//相手のコライダも球
			//タグは弾丸
			//親(弾丸)の親(武器)の親(キャラクタ)がプレイヤーなら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//30フレームかけてダウンし、繰り返さない
					ChangeAnimation(11, false, 30);
				}
			}
		}
		//自分のコライダのタグが体
		else if (m->GetTag() == CCollider::ETag::EBODY)
		{
			//相手のコライダも球
			//タグは弾丸
			//親(弾丸)の親(武器)の親(キャラクタ)がプレイヤーなら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//30フレームかけてダウンし、繰り返さない
					ChangeAnimation(11, false, 30);
				}
			}
		}
		//自分のコライダのタグが足
		else if (m->GetTag() == CCollider::ETag::ELEG)
		{
			//相手のコライダも球
			//タグは弾丸
			//親(弾丸)の親(武器)の親(キャラクタ)がプレイヤーなら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//30フレームかけてダウンし、繰り返さない
					ChangeAnimation(11, false, 30);
				}
			}
		}
		//自分のコライダのタグが索敵用
		else if (m->GetTag() == CCollider::ETag::ESEARCH)
		{
			//相手のコライダがプレイヤーのものなら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::ELEG &&
				o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//プレイヤーを見つけている
					IsFoundPlayer = true;
				}
				//衝突していないなら
				else
				{
					//プレイヤーを見つけていない
					IsFoundPlayer = false;
				}
			}
		}
		//相手が三角コライダかつ自分が索敵用以外の時
		if (o->GetType() == CCollider::EType::ETRIANGLE && m->GetTag() != CCollider::ETag::ESEARCH)
		{
			CVector adjust;//調整値
			//三角コライダと球コライダの衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				//衝突しない位置まで戻す
				mPosition = mPosition + adjust;
				IsGround = true;
			}
			else
			{
				IsGround = false;
			}
		}
		break;
	}
}

//球コライダの生成とボーンとの連動
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//足
	mColSphereLeg.SetMatrix(&mpCombinedMatrix[1]);
	//キャラ同士が重ならないための体コライダ
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//プレイヤー索敵用コライダ
	mColSphereSearch.SetMatrix(&mpCombinedMatrix[5]);
	//左手に引き金がくる数値
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}
