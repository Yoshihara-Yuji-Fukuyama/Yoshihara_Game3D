#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CActionCamera.h"
#include "CWepon.h"

class CXPlayer : public CXCharacter
{
public:
	CXPlayer();
	//インスタンスの取得
	static CXPlayer* GetInstance();

	//更新
	//移動、攻撃入力
	//変換行列、アニメーション、カプセルコライダの更新
	void Update();
	//武器の更新
	void WeponUpdate();
	//武器の描画
	void WeponRender();
	//マウス移動でY軸回転
	//turnHorizontal:横の回転量
	void Turn(float turnHorizontal);
	//衝突判定
	void Collision(CCollider* m, CCollider* o);
	//初期設定
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	static CXPlayer* spInstance;//インスタンス
	CCollider mColSphereHead; //頭
	CCollider mColSphereBody; //体
	//CColliderCapsule mColBody;//キャラとキャラが重ならないための体コライダ
	CCollider mColSphereSword;//剣
	CInput mInput;//CInputのインスタンス作成
	//右回転、左回転をしたかどうか
	bool IsLeftTurn;
	bool IsRightTurn;
	bool IsGround;//地面についているか
	bool IsJump;//ジャンプしているかどうか
	float mJumpPower;//ジャンプ力
	CWepon mWepon;
};
#endif