#include "CWepon.h"
#include "CXPlayer.h"
#include "CBullet.h"
#include "CActionCamera.h"

#define AR_OBJ "res\\Guns\\AssaultRifle2_1.obj"
#define AR_MTL "res\\Guns\\AssaultRifle2_1.mtl"

CModel CWepon::sModel;//モデルデータ作成

//デフォルトコンストラクタ
CWepon::CWepon()
	: mWeponType(EWeponType::EAR)
	, mpMatrix(&mMatrix)
	, mpRotation(nullptr)
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
	//後ろ移動をしていないなら
	if (IsRun == true)
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

void CWepon::ShotBullet()
{
	//弾を発射します
	CBullet* bullet = new CBullet(this);
	bullet->SetScale(25.0f);
	bullet->SetPosition(CVector(4.0f, 0.5f, 0.0f) * mMatrix);

	//発射される方向
	bullet->SetRotation(
		CVector(CActionCamera::GetInstance()->GetRotation().GetX() + 200.0f,
			mRotation.GetY() - 90.0f,
			mRotation.GetZ()));

	bullet->Update();
}

//走っているかどうかを設定
void CWepon::SetRun(bool isRun)
{
	IsRun = isRun;
}

