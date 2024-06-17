#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//キャラクタクラスのインクルード
#include "CXCharacter.h"
class CCollisionManager;

/*
コライダクラス
衝突判定データ
*/
class CCollider :public CTransform, public CTask
{
	friend CCollisionManager;
public:
	//コライダタイプ
	enum class EType
	{
		ESPHERE,  //球コライダ
		ETRIANGLE,//三角コライダ
		ELINE,    //線分コライダ
		ECAPSULE,  //カプセルコライダ
	};
	//mTypeの値を返す
	CCollider::EType GetType();
	//タグ
	enum class ETag
	{
		EHEAD, //頭
		EBODY, //体
		ELEG,  //足
		ESEARCH,//索敵用
		EBULLET,//弾丸
	};
	//mTagの値を返す
	ETag GetTag();

	CCollider();
	~CCollider();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="position">位置</param>
	/// <param name="radius">半径</param>
	/// <param name="tag">タグ</param>
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius, ETag tag = ETag::EBODY);
	void Render();

	//優先度の変更
	virtual void ChangePriority();
	//優先度の変更
	void ChangePriority(int priority);

	/// <summary>
	/// コライダ1と2の衝突判定
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	/// <returns>衝突の可否</returns>
	static bool Collision(CCollider* m, CCollider* o);
	/// <summary>
	/// 三角コライダと球コライダの衝突判定
	/// </summary>
	/// <param name="triangle">三角コライダ</param>
	/// <param name="sphere">球コライダ</param>
	/// <param name="adjust">調整値(衝突しない位置まで戻す値)</param>
	/// <returns>衝突の可否</returns>
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere, CVector* adjust);
	/// <summary>
	/// 三角コライダと線分コライダの衝突判定
	/// </summary>
	/// <param name="triangle">三角コライダ</param>
	/// <param name="line">線分コライダ</param>
	/// <param name="adjust">調整値(衝突しない位置まで戻す値)</param>
	/// <returns>衝突の可否</returns>
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
	/// <summary>
	/// カプセルコライダとカプセルコライダの衝突判定
	/// </summary>
	/// <param name="m">カプセルコライダ</param>
	/// <param name="o">カプセルコライダ</param>
	/// <param name="adjust">調整値</param>
	/// <returns>衝突の可否</returns>
	static bool CollisionCapsuleCapseule(CCollider* m, CCollider* o, CVector* adjust);

	//配列mV[i]の要素を返す
	const CVector& GetV(int i);
	//親ポインタの取得
	CCharacter3* GetParent();
	//親行列の設定
	void SetMatrix(CMatrix* m);
protected:
	EType mType;  //コライダタイプ
	ETag mTag;    //タグ
	CVector mV[3];//頂点
	CCharacter3* mpParent;//親
	CMatrix* mpMatrix;    //親行列
	float mRadius;//半径
};
#endif