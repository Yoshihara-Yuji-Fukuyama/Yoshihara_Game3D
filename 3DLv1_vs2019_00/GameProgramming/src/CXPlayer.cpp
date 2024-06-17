#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//回転速度
#define GRAVITY_AND_JUMPDEF 0.1f//重力とジャンプの基準値

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: mPlayerSpeed(0.1f)
	, IsGround(false)
	, IsJump(false)
	, mJumpPower(1.0f)
	, IsReloading(false)
	, mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f,CCollider::ETag::EHEAD)//頭,球コライダ
	, mColSphereLeg(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f,CCollider::ETag::ELEG)//足,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//体,カプセルコライダ
{
	//タグにプレイヤーを設定
	mCharaTag = ECharaTag::EPLAYER;
	//インスタンスに設定
	spInstance = this;
	//サイズ設定
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//前を向ける
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
}

//インスタンスの取得
CXPlayer* CXPlayer::GetInstance()
{
	return spInstance;
}

void CXPlayer::Update()
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

	//上昇中じゃないかつ地面に接触していないかつY座標が0より大きいなら重力適用
	if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() > -1.0f)
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
			mPosition = mPosition - charZ * mPlayerSpeed;
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

	//移動方向の設定
	CVector move;
	bool isMove = false;//動いているか
	bool isMoveB = false;//後ろ移動をしているか
	bool isAim = false;//構えているか
	//前進
	if (mInput.Key('W'))
	{
		move = move + cameraZ;
		isMove = true;
	}
	//後退
	else if (mInput.Key('S'))
	{
		move = move - cameraZ;
		isMove = true;
		isMoveB = true;
	}

	//左移動
	if (mInput.Key('A'))
	{
		move = move + cameraX;
		isMove = true;
	}
	//右移動
	else if (mInput.Key('D'))
	{
		move = move - cameraX;
		isMove = true;
	}

	//ジャンプ
	if (mInput.Key(VK_SPACE) && IsJump == false && IsReloading == false)
	{
		ChangeAnimation(7, false, 45 * mJumpPower);
		IsJump = true;
	}

	//左クリックが押されたら、弾丸を飛ばす
	if (mInput.Key(VK_LBUTTON) && IsRun == false && IsReloading == false)
	{
		//弾の生成
		mWepon.ShotBullet();
	}
	//右クリックが押されたら、構える
	if (mInput.Key(VK_RBUTTON) && IsRun == false && IsReloading == false)
	{
		ChangeDirection(charZ, cameraZ);
		isAim = true;
	}
	//Rキーが押されたら、弾を補充＋リロードアニメーション再生
	if (mInput.Key('R') && IsRun == false && IsReloading == false)
	{
		//動いているなら動きながらのリロードアニメーション
		if (isMove == true)
		{
			ChangeAnimation(11, false, 210);
			IsWalkReload = true;
		}
		//動いていないなら停止したリロードアニメーション
		else
		{
			ChangeAnimation(10, false, 90);
			IsWaitReload = true;
		}
		//リロード
		mWepon.Reload();
		IsReloading = true;
	}
	//移動中にShiftキーが押されたら、移動速度上昇し走る
	if (mInput.Key(VK_SHIFT) && isMove == true && IsJump == false && IsReloading == false)
	{
		if (IsRun == false)
		{
			mPlayerSpeed = mPlayerSpeed * 2.0f;
		}	
		IsRun = true;
		mWepon.SetRun(IsRun);
	}
	//押されていないなら、元の速度に戻る
	else
	{
		if (IsRun == true)
		{
			mPlayerSpeed = mPlayerSpeed / 2.0f;
		}
		IsRun = false;
		mWepon.SetRun(IsRun);
	}

	//移動あり、止まってリロード中は動けない
	if (move.Length() > 0.0f && IsWaitReload == false)
	{
		//正規化
		move = move.Normalize();
		if (IsWalkReload == false)
		{
			//移動方向へ移動
			mPosition = mPosition + move * mPlayerSpeed;
		}
		//他の動きをしていなければアニメーションを移動に変える
		if (IsRun == false && IsJump == false && IsReloading == false)
		{
			//前移動
			ChangeAnimation(0, true, 90 * (1 - (mPlayerSpeed * 0.1f)));
		}
		else if (IsJump == false && IsReloading == false)
		{
			//走り移動
			ChangeAnimation(9, true, 90 * (0.7f - (mPlayerSpeed * 0.1f)));
		}
		//構えていないなら
		if (isAim == false)
		{
			//移動方向を向かせる
			ChangeDirection(charZ, move);
		}
	}
	//移動もジャンプもリロードもしていない場合、待機アニメーションに切り替え
	//正面を向く
	else if (IsJump == false && IsReloading == false)
	{
		//待機アニメーションに変更
		ChangeAnimation(4, true, 90);
	}
	
	//変換行列、アニメーションの更新
	CXCharacter::Update();
	//カプセルコライダの更新
	mColBody.Update();
}

//武器の更新
void CXPlayer::WeponUpdate()
{
	mWepon.Update();
}

//武器の描画
void CXPlayer::WeponRender()
{
	mWepon.Render();
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

		//自分のコライダが球の場合
		//敵の攻撃と衝突したらダメージ
		//地面とぶつかると衝突しない位置まで戻す
	case CCollider::EType::ESPHERE:
		//自分のコライダのタグが頭
		if (m->GetTag() == CCollider::ETag::EBODY)
		{
			//相手のコライダも球
			//タグは弾丸
			//親(弾丸)の親(武器)の親(キャラクタ)が敵なら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
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
			//親(弾丸)の親(武器)の親(キャラクタ)が敵なら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
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
			//親(弾丸)の親(武器)の親(キャラクタ)が敵なら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::EBULLET &&
				o->GetParent()->GetParent()->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EENEMY)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//30フレームかけてダウンし、繰り返さない
					ChangeAnimation(11, false, 30);
				}
			}
		}
		//相手が三角コライダのとき
		if (o->GetType() == CCollider::EType::ETRIANGLE)
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

//初期設定
//球コライダの生成と、ボーンとの連動
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//足
	mColSphereLeg.SetMatrix(&mpCombinedMatrix[1]);
	//キャラ同士が重ならないための体コライダ 
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//左手に引き金がくる数値
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}


