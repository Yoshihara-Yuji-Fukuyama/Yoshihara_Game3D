#ifndef CWEPON_H
#define CWEPON_H

#include "CCharacter3.h"


class CWepon : public CCharacter3
{
public:
	//武器のタイプの種類
	enum class EWeponType
	{
		EAR, //0
		ESG, //1
		ESR, //2
		EHG, //3
		ESMG,//4
	};
	CWepon();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="parent">親行列</param>
	/// <param name="position">位置</param>
	/// <param name="rotation">回転</param>
	CWepon(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, CVector* rotation);
	~CWepon();
	void Update();
	//親行列の設定
	void SetMatrix(CMatrix* m);
	//弾を発射する関数
	void ShotBullet();
	//IsMoveBを設定する
	void SetMoveB(bool isMoveB);
private:
	static CModel sModel;//モデルデータ
	EWeponType mWeponType;//武器の種類
	CMatrix* mpMatrix;    //親行列
	CVector* mpRotation;  //親回転
	CVector mAdjustPosition;//位置調整
	bool IsMoveB;//後ろ移動をしているかどうかを保存する
};
#endif
