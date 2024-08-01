#ifndef CITEM_H
#define CITEM_H

#include "CCharacter3.h"
#include "CCollider.h"

//�G�����񂾂Ƃ��񕜃A�C�e���𗎂Ƃ�
class CItem : CCharacter3
{
public:
	CItem();
	CItem(const CVector& position);
	~CItem();
	void Update();
	//�v���C���[�ɐG���Ɖ񕜂��ď���
	void Collision(CCollider* m, CCollider* o);
private:
	CCollider mCol;//�����蔻��
	static CModel sModel;//���f���f�[�^
};
#endif
