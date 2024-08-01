#ifndef CITEM_H
#define CITEM_H

#include "CCharacter3.h"
#include "CCollider.h"

//敵が死んだとき回復アイテムを落とす
class CItem : CCharacter3
{
public:
	CItem();
	CItem(const CVector& position);
	~CItem();
	void Update();
	//プレイヤーに触れると回復して消去
	void Collision(CCollider* m, CCollider* o);
private:
	CCollider mCol;//当たり判定
	static CModel sModel;//モデルデータ
};
#endif
