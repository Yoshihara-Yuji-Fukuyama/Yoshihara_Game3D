#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//変換行列クラスのインクルード
#include "CTransform.h"
//モデルクラスのインクルード
#include "CModel.h"
#include "CTask.h"
class CCollider;
/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/
class CCharacter3 :public CTransform ,public CTask
{
public:
	//キャラタグ種類
	enum class ECharaTag
	{
		EPLAYER,//0
		EENEMY, //1
		EWEPON, //2
	};
	//自分のキャラタグを返す
	ECharaTag GetCharaTag();

	CCharacter3();
	CCharacter3(int priority);
	~CCharacter3();

	//衝突処理
	virtual void Collision(CCollider* m, CCollider* o) {}
	//モデルの設定
	//Model(CModel*m);
	void Model(CModel* m);
	//親ポインタの取得
	CCharacter3* GetParent();
	//描画処理
	void Render();
	/// <summary>
	/// キャラの方向を向かせたい方向に変える
	/// </summary>
	/// <param name="charZ">キャラの方向</param>
	/// <param name="direction">向かせたい方向</param>
	void ChangeDirection(CVector charZ, CVector direction, float margin = 0.06f);
protected:
	CModel* mpModel;//モデルのポインタ
	CCharacter3* mpParent;//親
	ECharaTag mCharaTag;   //自分のキャラタグ
};
#endif