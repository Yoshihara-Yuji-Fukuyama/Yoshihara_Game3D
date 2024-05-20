#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();

	//�X�V
	//�ړ��A�U������
	//�ϊ��s��A�A�j���[�V�����A�J�v�Z���R���C�_�̍X�V
	void Update();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //��
	CCollider mColSphereBody; //��
	CColliderCapsule mColBody;//�L�����ƃL�������d�Ȃ�Ȃ����߂̑̃R���C�_
	CCollider mColSphereSword0;//��
	CCollider mColSphereSword1;//��
	CCollider mColSphereSword2;//��
};
#endif