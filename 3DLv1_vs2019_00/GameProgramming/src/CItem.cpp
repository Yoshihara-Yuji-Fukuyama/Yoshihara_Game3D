#include "CItem.h"
#include "CXPlayer.h"

#define HEAL_OBJ "res\\HealItem.obj"
#define HEAL_MTL "res\\HealItem.mtl"
#define GRAVITY_AND_JUMPDEF 0.1f//重力

CModel CItem::sModel;

CItem::CItem()
	: mCol(this, &mMatrix, CVector(0.0f,0.5f,0.0f), 0.5f, CCollider::ETag::EITEM)
{
	//モデルを読み込んでないなら読み込む
	if (sModel.Triangles().size() == 0)
	{
		//アイテムモデルファイル読み込み
		sModel.Load(HEAL_OBJ, HEAL_MTL);
	}
	mpModel = &sModel;
	mCharaTag = ECharaTag::EITEM;
	mScale = CVector(1.0f, 1.0f, 1.0f);
}

CItem::CItem(const CVector& position)
	: CItem()
{
	mPosition = CVector(position.GetX(), 0, position.GetZ());
}

CItem::~CItem()
{
}

void CItem::Update()
{
	//Y座標が0より大きいなら重力適用
	if (mPosition.GetY() > -1.0f)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	CTransform::Update();
}

//プレイヤーに触れると回復して消去
void CItem::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
	case CCollider::EType::ESPHERE:
		//相手のコライダがカプセル
		//タグがプレイヤー
		if (o->GetType() == CCollider::EType::ECAPSULE&&
			o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
		{
			//衝突しているなら
			if (o->CollisionCapsuleSphere(o, m) == true)
			{
				CXPlayer::GetInstance()->Heal();
				mEnabled = false;
			}
		}
	}
}
