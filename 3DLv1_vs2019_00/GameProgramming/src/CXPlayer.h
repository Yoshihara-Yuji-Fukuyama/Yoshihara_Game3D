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
	//衝突判定
	void Collision(CCollider* m, CCollider* o);
	//初期設定
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	static CXPlayer* spInstance;//インスタンス
	CInput mInput;//CInputのインスタンス作成
	CWepon mWepon;//武器のインスタンス
	
	CCollider mColSphereHead; //頭
	CCollider mColSphereLeg;  //足
	CColliderCapsule mColBody;//キャラとキャラが重ならないための体コライダ
	
	float mPlayerSpeed;//プレイヤーの移動速度
	float mJumpPower;//ジャンプ力

	bool IsRun;//走っているかどうか
	bool IsGround;//地面についているか
	bool IsJump;//ジャンプしているかどうか
	bool IsReloading;//リロードしているか
	bool IsWalkReload;//歩いてリロードしているか
	bool IsWaitReload;//止まってリロードしているか
};
#endif