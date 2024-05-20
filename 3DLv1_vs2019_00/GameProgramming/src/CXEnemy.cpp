#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)//頭,球コライダ
	, mColSphereBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)//体,球コライダ
	, mColBody(this, nullptr, CVector(0.0f, -1.5f, 0.0f), CVector(0.0f, 1.0f, 0.0f), 1.0f)//体,カプセルコライダ
	, mColSphereSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//剣,カプセルコライダ
	, mColSphereSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//剣,カプセルコライダ
	, mColSphereSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f, CCollider::ETag::ESWORD)//剣,カプセルコライダ
{
	//タグにプレイヤーを設定
	mCharaTag = ECharaTag::EENEMY;
}


//更新
void CXEnemy::Update()
{
	//変換行列、アニメーションの更新
	CXCharacter::Update();
	//カプセルコライダの更新
	mColBody.Update();
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
		//自分のコライダのタグが体
		if (m->GetTag() == CCollider::ETag::EBODY)
		{
			//相手のコライダも球
			//タグは剣
			//親がプレイヤーなら
			if (o->GetType() == CCollider::EType::ESPHERE &&
				o->GetTag() == CCollider::ETag::ESWORD &&
				o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
			{
				//衝突しているなら
				if (m->Collision(m, o) == true)
				{
					//30フレームかけてダウンし、繰り返さない
					ChangeAnimation(11, false, 30);
				}

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
	mColSphereHead.SetMatrix(&mpCombinedMatrix[1]);
	//体
	mColSphereBody.SetMatrix(&mpCombinedMatrix[1]);
	mColBody.SetMatrix(&mpCombinedMatrix[1]);
	//剣
	mColSphereSword0.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword1.SetMatrix(&mpCombinedMatrix[26]);
	mColSphereSword2.SetMatrix(&mpCombinedMatrix[26]);
}
