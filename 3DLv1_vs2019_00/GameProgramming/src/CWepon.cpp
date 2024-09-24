#include "CWepon.h"
#include "CXPlayer.h"
#include "CBullet.h"
#include "CActionCamera.h"
#include <chrono>

#define AR_OBJ "res\\Guns\\AssaultRifle2_1.obj"
#define AR_MTL "res\\Guns\\AssaultRifle2_1.mtl"

#define AR_AMMO 30

CModel CWepon::sModel;//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CWepon::CWepon()
	: mWeponType(EWeponType::EAR)
	, mpMatrix(&mMatrix)
	, mpRotation(nullptr)
	, mAmmo(30)
	, mFireRate(0.5f)
{
	//���f�����Ȃ��Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(AR_OBJ, AR_MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
	//�^�O�ɃE�F�|����ݒ�
	mCharaTag = ECharaTag::EWEPON;
}

//�R���X�g���N�^
CWepon::CWepon(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, CVector* rotation)
	: CWepon()
{
	//�e����
	mpParent = parent;
	//�e�s��ݒ�
	mpMatrix = matrix;
	//�e�̉�]�ݒ�
	mpRotation = rotation;
	//���W�����p�̕ϐ��ݒ�
	mAdjustPosition = position;
	//�g�k�̐ݒ�
	mScale = CVector(0.5f, 0.5f, 0.5f);
	CTransform::Update();
}

CWepon::~CWepon()
{
}

//�e�̍��W���Q�Ƃ��A�e�̎茳�ɍ��W���X�V����
void CWepon::Update()
{
	mPosition = mAdjustPosition * *mpMatrix;
	mRotation = *mpRotation;
	//�W�����v�����Ă���Ȃ�
	if (IsJump == true)
	{
		mRotation.SetY(mRotation.GetY() + 90.0f);
	}
	//�����Ă���Ȃ�
	else if (IsRun == true)
	{
		//�e���͉E
		mRotation.SetY(mRotation.GetY() + 30.0f);
	}
	else
	{
		//�e���͑O
		mRotation.SetY(mRotation.GetY() + 90.0f);
	}

	CTransform::Update();
}

//�e�s��̐ݒ�
void CWepon::SetMatrix(CMatrix* m)
{
	mpMatrix = m;
}

//TODO:�G��\���ˌ���
//TODO:deltaTime���g����悤�ɂ���
//�e�𔭎˂���
void CWepon::ShotBullet()
{
	//mTimeSinceLastShot += deltaTime;
	//�e��1���ȏ゠�邩��
	//�Ō�Ɍ����������� (1/�A�ˑ��x) �b�o���Ă�����
	if (mAmmo > 0/* && mTimeSinceLastShot >= 1.0f / mFireRate*/)
	{
		//�e�𔭎˂��܂�
		CBullet* bullet = new CBullet(this);
		bullet->SetScale(25.0f);
		bullet->SetPosition(CVector(4.0f, 0.5f, 0.0f) * mMatrix);

		//�v���C���[�̏ꍇ
		if (mpParent->GetCharaTag() == ECharaTag::EPLAYER)
		{
			//���˂�������
			bullet->SetRotation(
				CVector(CActionCamera::GetInstance()->GetRotation().GetX() + 190.0f,
					mRotation.GetY() - 90.0f,
					mRotation.GetZ()));
		}
		//�G�̏ꍇ
		else
		{
			//���˂�������
			bullet->SetRotation(
				CVector(180.0f,
					mRotation.GetY() - 90.0f,
					mRotation.GetZ()));
			bullet->SetDirection(mPosition, CXPlayer::GetInstance()->GetPosition());
		}

		bullet->Update();
		//�e�������炷
		mAmmo--;
	}

#ifdef _DEBUG//�c�e���m�F
	printf("%d\n", mAmmo);
#endif
}

//�����[�h������
void CWepon::Reload()
{
	//���݂̒e����0���傫���Ȃ�
	if (mAmmo > 0)
	{
		//�e�����ő�+1�ɂ���
		mAmmo = AR_AMMO + 1;
	}
	else
	{
		//�c�e���ő�ɂ���
		mAmmo = AR_AMMO;
	}
#ifdef _DEBUG//�c�e���m�F
	printf("%d\n", mAmmo);
#endif
}

//�����Ă��邩�ǂ�����ݒ�
void CWepon::SetRun(bool isRun)
{
	IsRun = isRun;
}
//�W�����v�����Ă��邩�ǂ���
void CWepon::SetJump(bool isJump)
{
	IsJump = isJump;
}
//�c�e����Ԃ�
int CWepon::GetAmmo()
{
	return mAmmo;
}

