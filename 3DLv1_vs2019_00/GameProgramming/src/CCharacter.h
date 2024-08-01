#ifndef CCHARACTER_H
#define CCHARACTER_H

#include "CTexture.h"
#include "CRectangle.h"
#include "CTask.h"

//コライダクラスの宣言
class CCollider;

class CCharacter :public CRectangle, public CTask//親クラス名
{
public:

	enum class ETag //識別子
	{
		EPLAYER,	//プレイヤー
		ESLIME,		//スライム
		EONI,			//鬼
	};

	enum class EState//状態
	{
		EWAIT,    //待機
		EMOVE,    //移動
		EJUMP,    //ジャンプ
		EATTACK,	  //攻撃
		EHIT,     //敵の攻撃が当たった
		GUARD,	  //防御 
	};
	//デフォルトコンストラクタ
	CCharacter();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="priority">優先度</param>
	CCharacter(int priority);

	//デストラクタ
	virtual ~CCharacter();

	//mpTextureを返す
	CTexture* GetTexture();

	/// <summary>
	/// 有効フラグを設定
	/// </summary>
	/// <param name="isEnabled"></param>
	void SetEnabled(bool isEnabled);

	//有効フラグを返す
	bool GetEnabled();

	/// <summary>
	/// 処理順番を設定
	/// </summary>
	/// <param name="order">処理順番</param>
	void SetSortOrder(float order);

	//処理順番を取得
	float GetSortOrder();

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="pTexture">テクスチャのポインタ</param>
	/// <param name="left">テクスチャの左座標</param>
	/// <param name="right">テクスチャの右座標</param>
	/// <param name="bottom">テクスチャの下座標</param>
	/// <param name="top">テクスチャの上座標</param>
	void Texture(CTexture* pTexture, int left, int right, int bottom, int top);

	void Render();//描画

		//衝突処理2
	virtual void Collision() {};
	//衝突処理4
	virtual void Collision(CCharacter* m, CCharacter* o) {};

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	virtual void Collision(CCollider* m, CCollider* o) {}

	//足元の座標を取得
	float GetUnderPosY();
	//影の座標を取得
	float GetShadowPosY();

	//HPを取得
	float GetHp();
	//HPを設定
	void SetHp(float hp);

	//識別子の取得
	ETag GetTag();

	EState GetState();

	void SetLeg(float leg);

protected:
	ETag mTag;			//識別子の格納
	EState mState;      //状態
	bool isMove;        //移動しているか
	bool isMoveX;       //X軸移動しているか
	bool isMoveY;       //Y軸移動しているか
	bool isAttack;      //攻撃しているか
	bool isAttackNext;  //次も攻撃するか
	bool isHit;			//攻撃を受けているか
	bool isGuard;		//防御しているか
	bool isGeneration;	//コライダが生成されているか
	//足元計算用
	float mLeg;
	//影計算用
	float mShadow;
	//足元の座標
	float mUnderPosY;
	//影の座標
	float mShadowPosX;
	float mShadowPosY;
	//ジャンプ距離
	float mJump;
	//HP
	float mHp;
	//アニメーションごとの枚数
	int MoveNum;
	int AttackNum;
	int AttackNum2;
	int AttackNum3;
	int WaitNum;
	int JumpNum;
	int HitNum;
	int GuardNum;

	float mVx, mVy;		//速度
private:
	CTexture* mpTexture;//テクスチャ


	int mLeft, mRight, mBottom, mTop;//左、右、下、上

};
#endif