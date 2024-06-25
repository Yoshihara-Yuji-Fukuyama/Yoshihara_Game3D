#include "CBullet.h"
#include "CCollisionManager.h"

#define Bullet_Speed 1.0f

#define AR_OBJ "res\\Guns\\ammo_machinegun.obj"
#define AR_MTL "res\\Guns\\ammo_machinegun.mtl"

CModel CBullet::sModel;//モデルデータ作成

CBullet::CBullet(CCharacter3* parent)
	:mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.1f, CCollider::ETag::EBULLET)
{
	//モデルがないときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(AR_OBJ, AR_MTL);
	}
	//モデルのポインタ設定
	mpModel = &sModel;
	//親の設定
	mpParent = parent;
}

//大きさの設定
void CBullet::SetScale(float scale)
{
	//スケール設定
	mScale = CVector(scale, scale, scale);
}

//更新
void CBullet::Update()
{
	//キャラクタの前方
	CVector charZ = mMatrixRotate.GetVectorZ();
	//正規化
	charZ = charZ.Normalize();
	//生存時間の判定
	if (mLife-- > 0)
	{
		CTransform::Update();
		//位置更新
		//プレイヤーの弾は正面へ進んでいく
		if (mpParent->GetParent()->GetCharaTag() == ECharaTag::EPLAYER)
		{
			mPosition = mPosition + CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;
		}
		//敵の弾は移動方向へ進んでいく
		else if (mpParent->GetParent()->GetCharaTag() == ECharaTag::EENEMY)
		{
			mPosition = mPosition + mMoveDirection * Bullet_Speed;
		}
	}
	else
	{
		//無効にする
		mEnabled = false;
	}
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//相手が索敵用以外の時コライダのmとoが衝突しているか判定
	if (CCollider::Collision(m, o) && o->GetTag() != CCollider::ETag::ESEARCH)
	{
		//衝突しているときは無効にする
		mEnabled = false;
	}
}
//動く方向を設定
void CBullet::SetDirection(CVector m, CVector o)
{
	//動く方向を設定
	mMoveDirection = o - m;
	mMoveDirection = mMoveDirection.Normalize();
}

//衝突処理
void CBullet::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}



