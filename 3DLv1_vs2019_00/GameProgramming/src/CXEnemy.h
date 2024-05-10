#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();

	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //��
	CCollider mColSphereBody; //��
	CCollider mColSphereSword;//��
};
#endif