#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
public:
	CXPlayer();
	void Update();
	//初期設定
	//球コライダの生成とボーンとの連動
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //頭
	CCollider mColSphereBody; //体
	CCollider mColSphereSword;//剣
	CInput mInput;//CInputのインスタンス作成
};
#endif