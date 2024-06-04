#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//回転速度
#define PLAYER_SPEED 0.1f//移動速度
#define GRAVITY_AND_JUMPDEF 0.1f//重力とジャンプの基準値

CXPlayer* CXPlayer::spInstance = nullptr;

CXPlayer::CXPlayer()
	: IsLeftTurn(false)
	, IsRightTurn(false)
	, IsGround(false)
	, IsJump(false)
	, mJumpPower(1.0f)
	, mWepon(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)//頭,球コライダ
	, mColSphereBody(this, nullptr, CVector(), 0.5f)//体,球コライダ
	//, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 150.0f, 0.0f), 0.5f)//体,カプセルコライダ
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)//剣,球コライダ
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

	//上昇中じゃないかつY座標が0以上なら重力適用
	if (GetAnimationIndex() != 7 && mPosition.GetY() > 0)
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
	bool moveF = false;//前
	bool moveB = false;//後ろ
	bool moveL = false;//左
	bool moveR = false;//右
	//前進
	if (mInput.Key('W'))
	{
		move = move + cameraZ;
		moveF = true;
	}
	//後退
	else if (mInput.Key('S'))
	{
		move = move - cameraZ;
		moveB = true;
	}
	//左移動
	if (mInput.Key('A'))
	{
		move = move + cameraX;
		moveL = true;
	}
	//右移動
	else if (mInput.Key('D'))
	{
		move = move - cameraX;
		moveR = true;
	}
	//ジャンプ
	if (mInput.Key(VK_SPACE) && IsJump == false)
	{
		ChangeAnimation(7, false, 45 * mJumpPower);
		IsJump = true;
	}
	//左クリックが押されたら、弾丸を飛ばす
	if (mInput.Key(VK_LBUTTON))
	{

	}
	//Rキーが押されたら、弾を補充＋リロードアニメーション再生
	if (mInput.Key('R'))
	{

	}
	//移動あり
	if (move.Length() > 0.0f)
	{
		//正規化
		move = move.Normalize();
		//移動方向へ移動
		mPosition = mPosition + move * PLAYER_SPEED;
		//ジャンプ中でないなら
		//正面移動
		//アニメーション変更と方向変更
		if (moveF == true && IsJump == false)
		{

			//前移動
			ChangeAnimation(0, true, 90);
			//移動方向を向かせる
			ChangeDirection(charZ, move);
		}
		//ジャンプ中でないなら
		//後ろ移動
		//アニメーション変更と方向変更
		else if (moveB == true && IsJump == false)
		{
			//後ろ移動
			ChangeAnimation(1, true, 90);
			//移動方向の逆を向かせる
			ChangeDirection(charZ, move * -1);
		}
		//ジャンプ中でないなら
		//横移動
		//アニメーション変更と方向変更
		else if ((moveL == true || moveR == true) && IsJump == false)
		{
			//左移動なら
			if (moveL == true)
			{
				//左歩きアニメーションに変更
				ChangeAnimation(2, true, 90);
			}
			//右移動なら
			else if (moveR == true && IsJump == false)
			{
				//右歩きアニメーションに変更
				ChangeAnimation(3, true, 90);
			}
			//横移動中は正面を向かせる
			ChangeDirection(charZ, cameraZ);
		}
	}
	//移動もジャンプもしていない場合、待機アニメーションに切り替え
	//正面を向く
	else if (IsJump == false)
	{
		//待機アニメーションに変更
		ChangeAnimation(4, true, 90);
		//正面を向かせる
		ChangeDirection(charZ, cameraZ);
	}
	//ジャンプ中は正面を向かせる
	if (IsJump == true)
	{
		ChangeDirection(charZ, cameraZ);
	}
	//変換行列、アニメーションの更新
	CXCharacter::Update();
	//カプセルコライダの更新
	//mColBody.Update();
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
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[5]);
	//mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
	//TODO:左手に引き金がくる数値を探す
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

