#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMatrix.h"
#include "CCharacter3.h"

class CXCharacter : public CCharacter3 {
public:


	CXCharacter();
	virtual ~CXCharacter()
	{
		SAFE_DELETE_ARRAY(mpCombinedMatrix);
	}

	//更新処理
	void CXCharacter::Update();
	//初期化処理
	void Init(CModelX* model);
	/// <summary>
	/// アニメーションを切り替える
	/// </summary>
	/// <param name="index">アニメーション番号</param>
	/// <param name="loop">trueなら繰り返す</param>
	/// <param name="framesize">最後まで再生するのに使用されるフレーム数</param>
	void ChangeAnimation(int index, bool loop, float framesize);
	//更新処理
	//matrix:移動、回転、拡大縮小の行列
	void Update(CMatrix& matrix);
	//描画処理
	void Render();
	//アニメーションの再生終了判定
	//true:終了　false:再生中
	bool IsAnimationFinished();
	//アニメーション番号の取得
	int GetAnimationIndex();
	/// <summary>
	/// キャラの方向を向かせたい方向に変える
	/// </summary>
	/// <param name="charZ">キャラの方向</param>
	/// <param name="direction">向かせたい方向</param>
	void ChangeDirection(CVector charZ,CVector direction);
protected:
	CModelX* mpModel;//モデルデータ
	int mAnimationIndex;      //アニメーション番号
	bool mAnimationLoopFlg;   //true:アニメーションを繰り返す
	float mAnimationFrame;    //アニメーションの再生フレーム
	float mAnimationFrameSize;//アニメーションの再生フレーム数
	CMatrix* mpCombinedMatrix;//合成行列退避
};
#endif