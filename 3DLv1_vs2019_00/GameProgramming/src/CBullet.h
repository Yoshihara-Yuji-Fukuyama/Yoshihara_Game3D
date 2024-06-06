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
	//衝突処理
	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);

	CBullet();
	//幅と奥行きの設定
	//Set(幅、奥行)
	void Set(float w, float d);
	//更新
	void Update();
private:
	int mLife;//生存時間
	CCollider mCollider;//コライダ
	static CModel sModel;//モデルデータ
};
#endif