#include "CWepon.h"
#include "CXPlayer.h"
#include "CBullet.h"
#include "CActionCamera.h"
#include <chrono>

#define AR_OBJ "res\\Guns\\AssaultRifle2_1.obj"
#define AR_MTL "res\\Guns\\AssaultRifle2_1.mtl"

#define AR_AMMO 30

CModel CWepon::sModel;//モデルデータ作成

//デフォルトコンストラクタ
CWepon::CWepon()
	: mWeponType(EWeponType::EAR)
	, mpMatrix(&mMatrix)
	, mpRotation(nullptr)
	, mAmmo(30)
	, mFireRate(0.5f)
{
	//モデルがないときは読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(AR_OBJ, AR_MTL);
	}
	//モデルのポインタ設定
	mpModel = &sModel;
	//タグにウェポンを設定
	mCharaTag = ECharaTag::EWEPON;
}

//コンストラクタ
CWepon::CWepon(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, CVector* rotation)
	: CWepon()
{
	//親決定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//親の回転設定
	mpRotation = rotation;
	//座標調整用の変数設定
	mAdjustPosition = position;
	//拡縮の設定
	mScale = CVector(0.5f, 0.5f, 0.5f);
	CTransform::Update();
}

CWepon::~CWepon()
{
}

//親の座標を参照し、親の手元に座標を更新する
void CWepon::Update()
{
	mPosition = mAdjustPosition * *mpMatrix;
	mRotation = *mpRotation;
	//ジャンプをしているなら
	if (IsJump == true)
	{
		mRotation.SetY(mRotation.GetY() + 90.0f);
	}
	//走っているなら
	else if (IsRun == true)
	{
		//銃口は右
		mRotation.SetY(mRotation.GetY() + 30.0f);
	}
	else
	{
		//銃口は前
		mRotation.SetY(mRotation.GetY() + 90.0f);
	}

	CTransform::Update();
}

//親行列の設定
void CWepon::SetMatrix(CMatrix* m)
{
	mpMatrix = m;
}

//TODO:敵を予測射撃化
//TODO:deltaTimeが使えるようにする
//弾を発射する
void CWepon::ShotBullet()
{
	//mTimeSinceLastShot += deltaTime;
	//弾が1発以上あるかつ
	//最後に撃った時から (1/連射速度) 秒経っていたら
	if (mAmmo > 0/* && mTimeSinceLastShot >= 1.0f / mFireRate*/)
	{
		//弾を発射します
		CBullet* bullet = new CBullet(this);
		bullet->SetScale(25.0f);
		bullet->SetPosition(CVector(4.0f, 0.5f, 0.0f) * mMatrix);

		//プレイヤーの場合
		if (mpParent->GetCharaTag() == ECharaTag::EPLAYER)
		{
			//発射される方向
			bullet->SetRotation(
				CVector(CActionCamera::GetInstance()->GetRotation().GetX() + 190.0f,
					mRotation.GetY() - 90.0f,
					mRotation.GetZ()));
		}
		//敵の場合
		else
		{
			//発射される方向
			bullet->SetRotation(
				CVector(180.0f,
					mRotation.GetY() - 90.0f,
					mRotation.GetZ()));
			bullet->SetDirection(mPosition, CXPlayer::GetInstance()->GetPosition());
		}

		bullet->Update();
		//弾数を減らす
		mAmmo--;
	}

#ifdef _DEBUG//残弾を確認
	printf("%d\n", mAmmo);
#endif
}

//リロードをする
void CWepon::Reload()
{
	//現在の弾数が0より大きいなら
	if (mAmmo > 0)
	{
		//弾数を最大+1にする
		mAmmo = AR_AMMO + 1;
	}
	else
	{
		//残弾を最大にする
		mAmmo = AR_AMMO;
	}
#ifdef _DEBUG//残弾を確認
	printf("%d\n", mAmmo);
#endif
}

//走っているかどうかを設定
void CWepon::SetRun(bool isRun)
{
	IsRun = isRun;
}
//ジャンプをしているかどうか
void CWepon::SetJump(bool isJump)
{
	IsJump = isJump;
}
//残弾数を返す
int CWepon::GetAmmo()
{
	return mAmmo;
}

