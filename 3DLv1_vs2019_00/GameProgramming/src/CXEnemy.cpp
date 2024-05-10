#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
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
	mColSphereHead.SetMatrix(&mpCombinedMatrix[11]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[8]);
	//剣
	mColSphereSword.SetMatrix(&mpCombinedMatrix[21]);
}
