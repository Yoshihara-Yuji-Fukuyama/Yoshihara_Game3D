#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//三角形クラスのインクルード
#include "CTriangle.h"
//コライダクラスのインクルード
#include "CCollider.h"

/*
弾クラス
三角形を飛ばす
*/
class CBullet : public CCharacter3
{
public:
	//親を設定
	CBullet(CCharacter3* parent);
	//大きさの設定
	void SetScale(float scale);
	//更新
	void Update();

	//衝突処理
	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//動く方向を設定
	void SetDirection(CVector m, CVector o);
private:
	int mLife;//生存時間
	CVector mMoveDirection;//動く方向
	CCollider mCollider;//コライダ
	static CModel sModel;//モデルデータ
};
#endif