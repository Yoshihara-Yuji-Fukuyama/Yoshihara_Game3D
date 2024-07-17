#include "CXEnemy.h"
#include "CXPlayer.h"
#include "CApplication.h"

#define GRAVITY_AND_JUMPDEF 0.1f//重力とジャンプの基準値

CModelX CXEnemy::sModel;

CXEnemy::CXEnemy()
	: mWepon(this, &mMatrix, CVector(-10.0f, 5.0f, -5.0f), &mRotation)
	, mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f, CCollider::ETag::EHEAD)//頭,球コライダ
	, mColSphereBody(this, nullptr, CVector(0.0f, 0.0f, 0.0f), 0.5f)//体,球コライダ
	, mColSphereLeg(this, nullptr, CVector(0.0f, 25.0f, 0.0f), 0.5f, CCollider::ETag::ELEG)//足,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, 25.0f, 0.0f), CVector(0.0f, 130.0f, 0.0f), 0.5f)//体,カプセルコライダ
	, mColSphereSearch(this, nullptr, CVector(), 60.0f, CCollider::ETag::ESEARCH)//索敵用、球コライダ
{
	//モデルを読み込んでないなら読み込む
	if (sModel.IsLoaded() == false)
	{
		//敵3Dモデルファイル読み込み
		sModel.Load(MODEL_PLAYER);//0:前歩き
		//追加アニメーション読み込み
		sModel.AddAnimationSet(BACKWARD); //1:後ろ歩き
		sModel.AddAnimationSet(L_WALK);   //2:左歩き
		sModel.AddAnimationSet(R_WALK);   //3:右歩き
		sModel.AddAnimationSet(AIM_IDLE); //4:構え待機
		sModel.AddAnimationSet(Fire);     //5:射撃
		sModel.AddAnimationSet(JUMP_UP);  //6:ジャンプ
		sModel.SeparateAnimationSet(6, 5, 16, "JumpUp");//7:ジャンプ上昇
		sModel.AddAnimationSet(JUMP_DOWN);//8:ジャンプ降下
		sModel.AddAnimationSet(RUN);//9:走り
		sModel.AddAnimationSet(IDLE_RELOAD);//10:止まってリロード
		sModel.AddAnimationSet(WALK_RELOAD);//11:歩きながらリロード
		sModel.AddAnimationSet(HIT);//12:被弾
		sModel.AddAnimationSet(WALK_HIT);//13:歩きながら被弾
		sModel.AddAnimationSet(DEATH);//14:死亡
	}
	//モデルの設定
	Init(&sModel);
	//AI初期設定
	CEnemyAi::Init();
	//タグに敵を設定
	mCharaTag = ECharaTag::EENEMY;
	//サイズ設定
	SetScale(CVector(0.025f, 0.025f, 0.025f));
	//前を向ける
	SetRotation(CVector(0.0f, 180.0f, 0.0f));
	//始まりの時間を保存
	start = clock();
	//秒数カウンタを1に
	mCount = 1;
}
//座標を設定
CXEnemy::CXEnemy(CVector pos)
	: CXEnemy()
{
	mPosition = pos;
}

//球コライダの生成とボーンとの連動
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[7]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[7]);
	//足
	mColSphereLeg.SetMatrix(&mpCombinedMatrix[1]);
	//キャラ同士が重ならないための体コライダ
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//プレイヤー索敵用コライダ
	mColSphereSearch.SetMatrix(&mpCombinedMatrix[5]);
	//左手に引き金がくる数値
	mWepon.SetMatrix(&mpCombinedMatrix[38]);
}

//敵の生成中数の数字を減らす
CXEnemy::~CXEnemy()
{
	CApplication::GetInstance()->DeathEnemy();
}


//更新
//敵の動き
void CXEnemy::Update()
{
	//上昇中じゃないかつ地面に接触していないかつY座標が-1以上なら重力適用
	if (GetAnimationIndex() != 7 && IsGround == false && mPosition.GetY() >= -1.0f)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	//HPが0以下なら死亡
	if (mHp <= 0)
	{
		ChangeState(CEnemyAi::EAiState::EDIE);
	}

	switch (mAiState)
	{
	case CEnemyAi::EAiState::EWAIT://待機
		Wait();
		break;
	case CEnemyAi::EAiState::EWANDERING://徘徊
		Wandering();
		break;
	case CEnemyAi::EAiState::ECHASE://追跡
		Chase();
		break;
	case CEnemyAi::EAiState::EATTACK://攻撃
		Attack();
		break;
	case CEnemyAi::EAiState::EMOVEATTACK://移動しながら攻撃
		MoveAttack();
		break;
	case CEnemyAi::EAiState::ERELOAD://リロード
		Reload();
		break;
	case CEnemyAi::EAiState::EESCAPE://逃亡
		Escape();
		break;
	case CEnemyAi::EAiState::EDAMAGE://被弾
		Damage();
		break;
	case CEnemyAi::EAiState::EDIE://死亡
		Die();
		break;
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
		//敵の攻撃と衝突したらダメージ
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
					mHp--;
					//被弾アニメーション
					ChangeAnimation(12, false, 30);
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
					mHp--;
					//被弾アニメーション
					ChangeAnimation(12, false, 30);
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
					mHp--;
					//被弾アニメーション
					ChangeAnimation(12, false, 30);
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

bool CXEnemy::IsTime(int lag)
{
	clock_t end = clock();//経過時間
	double sec = (double)(end - start) / CLOCKS_PER_SEC;//秒数変換
	if (sec >= mCount)
	{
		mCount = mCount + lag;
		return true;
	}
	return false;
}

void CXEnemy::Wait()//待機
{
	//待機アニメーション
	ChangeAnimation(4, true, 90);
	if (IsFoundPlayer == true)
	{
		ChangeState(CEnemyAi::EAiState::ECHASE);
	}
}

void CXEnemy::Wandering()//徘徊
{
	//プレイヤーを見つけたなら、追跡
	if (IsFoundPlayer == true)
	{
		ChangeState(CEnemyAi::EAiState::ECHASE);
	}
}

void CXEnemy::Chase()//追跡
{
	//キャラクタの前方
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ平面にして正規化
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//プレイヤーの方へ進む
	//プレイヤーへの方向を計算
	CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
	//プレイヤーとの距離が30以上の場合近づく
	if (moveDirection.Length() > 30.0f)
	{
		moveDirection = moveDirection.Normalize();
		mPosition = mPosition - moveDirection * mSpeed;
		ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
	}
	//それより近ければ攻撃
	else
	{
		ChangeState(CEnemyAi::EAiState::EATTACK);
	}
	//移動方向を向かせる
	ChangeDirection(charZ, moveDirection * -1, 0.06f);

	if (IsFoundPlayer == false)
	{
		ChangeState(CEnemyAi::EAiState::EWANDERING);
	}
}

void CXEnemy::Attack()//攻撃
{
	//キャラクタの前方
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ平面にして正規化
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//プレイヤーの方へ進む
	//プレイヤーへの方向を計算
	CVector moveDirection = mPosition - CXPlayer::GetInstance()->GetPosition();
	//プレイヤーの方向を向かせる
	ChangeDirection(charZ, moveDirection * -1, 0.06f);
	//射撃
	mWepon.ShotBullet();
	//残弾数が0ならリロード
	if (mWepon.GetAmmo() == 0)
	{
		ChangeState(CEnemyAi::EAiState::ERELOAD);
	}
}

void CXEnemy::MoveAttack()
{
	//射撃
	mWepon.ShotBullet();
	//残弾数が0ならリロード
	if (mWepon.GetAmmo() == 0)
	{
		ChangeState(CEnemyAi::EAiState::ERELOAD);
	}
}

void CXEnemy::Reload()
{
	//弾が切れたら、弾を補充＋リロードアニメーション再生
	if (mWepon.GetAmmo() == 0)
	{
		//リロードモーション
		ChangeAnimation(10, false, 90);
		mWepon.Reload();
	}
	//リロードアニメーションの時
	if (GetAnimationIndex() == 10)
	{
		//アニメーションが終了したら
		//待機ステートにする
		if (IsAnimationFinished() == true)
		{
			ChangeState(CEnemyAi::EAiState::EWAIT);
		}
	}
}

void CXEnemy::Escape()
{
	//キャラクタの前方
	CVector charZ = mMatrixRotate.GetVectorZ();
	//XZ平面にして正規化
	charZ.SetY(0.0f); charZ = charZ.Normalize();
	//プレイヤーと逆の方へ進む
	//プレイヤーと逆への方向を計算
	CVector moveDirection = CXPlayer::GetInstance()->GetPosition() - mPosition;
	//正規化
	moveDirection = moveDirection.Normalize();
	mPosition = mPosition - moveDirection * mSpeed;
	ChangeAnimation(0, true, 90 * (1 - (mSpeed * 0.1f)));
	//移動方向を向かせる
	ChangeDirection(charZ, moveDirection * -1, 0.6f);

	//プレイヤーを見つけていなければ待機に移行
	if (IsFoundPlayer == false)
	{
		ChangeState(CEnemyAi::EAiState::EWAIT);
	}
}

void CXEnemy::Damage()
{
	//被弾アニメーションの時
	if (GetAnimationIndex() == 12)
	{
		//アニメーションが終了したら
		//待機ステートにする
		if (IsAnimationFinished() == true)
		{
			ChangeState(CEnemyAi::EAiState::EWAIT);
		}
	}
}

void CXEnemy::Die()
{
	ChangeAnimation(14, false, 90);
	//死亡アニメーションが終わったら消去
	if (GetAnimationIndex() == 14)
	{
		if (IsAnimationFinished())
		{
			mEnabled = false;
		}
	}
}
