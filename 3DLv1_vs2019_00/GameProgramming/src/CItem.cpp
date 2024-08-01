#include "CItem.h"
#include "CXPlayer.h"

#define HEAL_OBJ "res\\HealItem.obj"
#define HEAL_MTL "res\\HealItem.mtl"
#define GRAVITY_AND_JUMPDEF 0.1f//�d��

CModel CItem::sModel;

CItem::CItem()
	: mCol(this, &mMatrix, CVector(0.0f,0.5f,0.0f), 0.5f, CCollider::ETag::EITEM)
{
	//���f����ǂݍ���łȂ��Ȃ�ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		//�A�C�e�����f���t�@�C���ǂݍ���
		sModel.Load(HEAL_OBJ, HEAL_MTL);
	}
	mpModel = &sModel;
	mCharaTag = ECharaTag::EITEM;
	mScale = CVector(1.0f, 1.0f, 1.0f);
}

CItem::CItem(const CVector& position)
	: CItem()
{
	mPosition = CVector(position.GetX(), 0, position.GetZ());
}

CItem::~CItem()
{
}

void CItem::Update()
{
	//Y���W��0���傫���Ȃ�d�͓K�p
	if (mPosition.GetY() > -1.0f)
	{
		mPosition.SetY(mPosition.GetY() - GRAVITY_AND_JUMPDEF);
	}
	CTransform::Update();
}

//�v���C���[�ɐG���Ɖ񕜂��ď���
void CItem::Collision(CCollider* m, CCollider* o)
{
	switch (m->GetType())
	{
	case CCollider::EType::ESPHERE:
		//����̃R���C�_���J�v�Z��
		//�^�O���v���C���[
		if (o->GetType() == CCollider::EType::ECAPSULE&&
			o->GetParent()->GetCharaTag() == CCharacter3::ECharaTag::EPLAYER)
		{
			//�Փ˂��Ă���Ȃ�
			if (o->CollisionCapsuleSphere(o, m) == true)
			{
				CXPlayer::GetInstance()->Heal();
				mEnabled = false;
			}
		}
	}
}
