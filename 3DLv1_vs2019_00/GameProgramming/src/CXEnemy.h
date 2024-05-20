#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();

	//更新
	//移動、攻撃入力
	//変換行列、アニメーション、カプセルコライダの更新
	void Update();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //頭
	CCollider mColSphereBody; //体
	CColliderCapsule mColBody;//キャラとキャラが重ならないための体コライダ
	CCollider mColSphereSword0;//剣
	CCollider mColSphereSword1;//剣
	CCollider mColSphereSword2;//剣
};
#endif