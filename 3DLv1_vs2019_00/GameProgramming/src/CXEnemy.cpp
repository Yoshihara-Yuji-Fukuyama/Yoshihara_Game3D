#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)
{
	mCharaTag = ECharaTag::EENEMY;
}

//衝突処理
void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//衝突しているなら
	if (m->Collision(m, o) == true)
	{
		//自分のコライダタイプが球でタグが体
		//相手のコライダタイプが球でタグが剣、親がプレイヤーなら
		if (m->GetType() == CCollider::EType::ESPHERE && m->GetTag() == CCollider::ETag::EBODY &&
			o->GetType() == CCollider::EType::ESPHERE && o->GetTag() == CCollider::ETag::ESWORD &&
			o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
		{
			//30フレームかけてダウンし、繰り返さない
			ChangeAnimation(11, false, 30);
		}
	}
}

//球コライダの生成とボーンとの連動
void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.SetMatrix(&mpCombinedMatrix[1]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword0.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword1.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword2.SetMatrix(&mpCombinedMatrix[26]);
}
