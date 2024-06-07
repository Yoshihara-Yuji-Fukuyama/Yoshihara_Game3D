#include "CBullet.h"
#include "CCollisionManager.h"

#define VELOCITY CVector(0.0f,0.0f,1.0f)

#define AR_OBJ "res\\Guns\\ammo_machinegun.obj"
#define AR_MTL "res\\Guns\\ammo_machinegun.mtl"

CModel CBullet::sModel;//モデルデータ作成

CBullet::CBullet(CCharacter3* parent)
	:mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.1f, CCollider::ETag::EBullet)
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
	//生存時間の判定
	if (mLife-- > 0)
	{
		CTransform::Update();
		//位置更新
		mPosition = mPosition + VELOCITY * mMatrixRotate;
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
	//コライダのmとoが衝突しているか判定
	if (CCollider::Collision(m, o))
	{
		//衝突しているときは無効にする
		mEnabled = false;
	}
}

//衝突処理
void CBullet::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}



