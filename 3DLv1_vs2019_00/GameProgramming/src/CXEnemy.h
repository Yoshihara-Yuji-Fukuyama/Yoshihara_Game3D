#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CWepon.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();
	//更新
	//移動、攻撃入力
	//変換行列、アニメーション、カプセルコライダの更新
	void Update();
	//武器の更新
	void WeponUpdate();
	//武器の描画
	void WeponRender();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	CWepon mWepon;//武器のインスタンス

	CCollider mColSphereHead; //頭
	CCollider mColSphereLeg;  //足
	CColliderCapsule mColBody;//キャラとキャラが重ならないための体コライダ
	CCollider mColSphereSearch;//プレイヤー索敵用コライダ

	float mEnemySpeed;//敵の移動速度
	float mJumpPower;//ジャンプ力

	bool IsMove;//動いているかどうか
	bool IsRun;//走っているかどうか
	bool IsGround;//地面についているか
	bool IsJump;//ジャンプしているかどうか
	bool IsReloading;//リロードしているか
	bool IsWalkReload;//歩いてリロードしているか
	bool IsWaitReload;//止まってリロードしているか

	bool IsFoundPlayer;//プレイヤーを見つけている
	bool IsInRange;    //射程内にプレイヤーがいる
};
#endif