#include "CCharacter3.h"
#include "CApplication.h"

//モデルの設定
void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}
//親のポインタを取得
CCharacter3* CCharacter3::GetParent()
{
	return mpParent;
}

//描画処理
void CCharacter3::Render()
{
	mpModel->Render(mMatrix);
}

CCharacter3::~CCharacter3()
{
	//タスクリストから削除
	CTaskManager::GetInstance()->Remove(this);
}

CCharacter3::ECharaTag CCharacter3::GetCharaTag()
{
	return mCharaTag;
}

CCharacter3::CCharacter3()
	:mpModel(nullptr)
{
	//タスクリストに追加
	CTaskManager::GetInstance()->Add(this);
}

CCharacter3::CCharacter3(int priority)
	:mpModel(nullptr)
{
	mPriority = priority;
	CTaskManager::GetInstance()->Add(this);
}

//キャラの方向を向かせたい方向に変える
void CCharacter3::ChangeDirection(CVector charZ, CVector direction, float margin)
{
	//遊び
	float MARGIN = margin;
	//自分の向きと向かせたい向きで外積
	float cross = charZ.Cross(direction).GetY();
	//自分の向きと向かせたい向きで内積
	float dot = charZ.Dot(direction);
	//外積がプラスは右回転
	if (cross > MARGIN)
	{
		mRotation.SetY(mRotation.GetY() - 5.0f);
	}
	//外積がマイナスは左回転
	else if (cross < -MARGIN)
	{
		mRotation.SetY(mRotation.GetY() + 5.0f);
	}
	//前後の向きが同じとき内積は1.0f
	//向かせたい方向を向いていたら何もしない
	else if (dot < 1.0f - MARGIN)
	{

	}
}