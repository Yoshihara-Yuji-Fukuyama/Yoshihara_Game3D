#include "CCharacter.h"
#include "CTaskManager.h"

//テクスチャの設定
void CCharacter::Texture(CTexture* pTexture, int left, int right, int bottom, int top)
{
	mpTexture = pTexture;

	mLeft = left;

	mRight = right;

	mBottom = bottom;

	mTop = top;
}

//描画
void CCharacter::Render()
{
	mpTexture->DrawImage(
		GetX() - GetW(),
		GetX() + GetW(),
		GetY() - GetH(),
		GetY() + GetH(),
		mLeft, mRight, mBottom, mTop
	);
}

//デフォルトコンストラクタ
CCharacter::CCharacter()
	: mpTexture(nullptr)
	, mLeft(0.0f), mRight(0.0f), mBottom(0.0f), mTop(0.0f)
{
	mPriority = 0;
	CTaskManager::GetInstance()->Add(this);
}

//コンストラクタ
CCharacter::CCharacter(int priority)
	: mpTexture(nullptr)
	, mLeft(0.0f), mRight(0.0f), mBottom(0.0f), mTop(0.0f)
{
	//優先順位の設定
	mPriority = priority;
	//タスクマネージャーへ追加
	CTaskManager::GetInstance()->Add(this);
}

//デストラクタ
//削除されたらリストからも削除する
CCharacter::~CCharacter()
{
	CTaskManager::GetInstance()->Remove(this);
}

//mpTextureを返す
CTexture* CCharacter::GetTexture()
{
	return mpTexture;
}

//有効フラグを設定
void CCharacter::SetEnabled(bool isEnabled)
{
	mEnabled = isEnabled;
}
//有効フラグを返す
bool CCharacter::GetEnabled()
{
	return mEnabled;
}

//処理順番を設定
void CCharacter::SetSortOrder(float order)
{
	mSortOrder = order;
	CTaskManager::GetInstance()->Remove(this, true);
	CTaskManager::GetInstance()->Add(this, true);
}
//処理順番を取得
float CCharacter::GetSortOrder()
{
	return mSortOrder;
}

//足元の座標を取得
float CCharacter::GetUnderPosY()
{
	if (mState != EState::EJUMP)
	{
		mUnderPosY = GetY() - mLeg;
	}
	else
	{
		mUnderPosY = mJump;
	}
	return mUnderPosY;
}

//影の座標を取得
float CCharacter::GetShadowPosY()
{
	if (mState != EState::EJUMP)
	{
		mShadowPosY = GetY() - mShadow;
	}
	return mShadowPosY;
}

//HPを取得
float CCharacter::GetHp()
{
	return mHp;
}
//HPを設定
void CCharacter::SetHp(float hp)
{
	mHp = hp;
}

CCharacter::ETag CCharacter::GetTag()
{
	return mTag;
}

//状態の取得
CCharacter::EState CCharacter::GetState()
{
	return mState;
}

void CCharacter::SetLeg(float leg)
{
	mLeg = leg;
}