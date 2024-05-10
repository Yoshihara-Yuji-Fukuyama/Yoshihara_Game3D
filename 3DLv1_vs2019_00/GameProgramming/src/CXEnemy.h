#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();

	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //頭
	CCollider mColSphereBody; //体
	CCollider mColSphereSword;//剣
};
#endif