#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CWepon.h"
#include "CEnemyAi.h"
#include <time.h>

class CXEnemy : public CXCharacter , public CEnemyAi
{
public:
	CXEnemy();
	CXEnemy(CVector pos);	//座標を設定
	void Init(CModelX* model);	//球コライダの生成とボーンとの連動
	~CXEnemy();	//敵の生成中数の数字を減らす

	void Update();	//更新

	void WeponUpdate();	//武器の更新
	void WeponRender();	//武器の描画
	
	void Collision(CCollider* m, CCollider* o);	//衝突処理

	bool IsTime(int lag);//lag秒ごとにtrueを返す

	//行動系
	void Wait();      //待機
	void Wandering(); //徘徊
	void Chase();     //追跡
	void Attack();    //攻撃
	void MoveAttack();//動きながら攻撃
	void Reload();    //リロード
	void Escape();    //逃亡
	void Damage();    //被弾
	void Die();       //死亡
private:
	CWepon mWepon;//武器のインスタンス

	CCollider mColSphereHead; //頭
	CCollider mColSphereBody; //体
	CCollider mColSphereLeg;  //足
	CColliderCapsule mColBody;//キャラとキャラが重ならないための体コライダ
	CCollider mColSphereSearch;//プレイヤー索敵用コライダ

	clock_t start;//始まりの時間を保存
	double mCount;//秒数カウンタ


	static CModelX sModel;	//モデルデータ

	bool IsFoundPlayer;//プレイヤーを見つけている
	bool IsInRange;    //射程内にプレイヤーがいる
	bool IsAttack;     //攻撃できるか
};
#endif