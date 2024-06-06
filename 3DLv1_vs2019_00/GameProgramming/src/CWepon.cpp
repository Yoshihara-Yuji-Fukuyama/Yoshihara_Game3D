#include "CWepon.h"
#include "CXPlayer.h"
#include "CBullet.h"
#include "CActionCamera.h"

#define AR_OBJ "res\\Guns\\AssaultRifle2_1.obj"
#define AR_MTL "res\\Guns\\AssaultRifle2_1.mtl"

CModel CWepon::sModel;//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CWepon::CWepon()
	: mWeponType(EWeponType::EAR)
	, mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mpRotation(nullptr)
	, IsMoveB(false)
{
	//���f�����Ȃ��Ƃ��͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(AR_OBJ, AR_MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &sModel;
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
	//���ړ������Ă��Ȃ��Ȃ�
	if (IsMoveB == false)
	{
		//�e���͑O
		mRotation.SetY(mRotation.GetY() + 90.0f);
	}
	//���Ă���Ȃ�
	else
	{
		//�e���͉E��
		mRotation.SetZ(mRotation.GetZ() - 15.0f);
	}
	CTransform::Update();
}

//�e�s��̐ݒ�
void CWepon::SetMatrix(CMatrix* m)
{
	mpMatrix = m;
}

void CWepon::ShotBullet()
{
	//�e�𔭎˂��܂�
	CBullet* bullet = new CBullet();
	bullet->Set(0.1f, 1.5f);
	bullet->SetPosition(CVector(4.0f, 0.0f, 0.0f) * mMatrix);
	bullet->SetRotation(
		CVector(CActionCamera::GetInstance()->GetRotation().GetX() + 200.0f ,
		CActionCamera::GetInstance()->GetRotation().GetY(),
		CActionCamera::GetInstance()->GetRotation().GetZ()));
	bullet->Update();
}
//���ړ������Ă��邩�ǂ�����ݒ肷��
void CWepon::SetMoveB(bool isMoveB)
{
	IsMoveB = isMoveB;
}

