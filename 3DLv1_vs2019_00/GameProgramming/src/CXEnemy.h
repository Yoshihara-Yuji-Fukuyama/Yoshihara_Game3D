#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CWepon.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();
	CXEnemy(CVector pos);
	//�X�V
	//�ړ��A�U������
	//�ϊ��s��A�A�j���[�V�����A�J�v�Z���R���C�_�̍X�V
	void Update();
	//����̍X�V
	void WeponUpdate();
	//����̕`��
	void WeponRender();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	void Init(CModelX* model);
private:
	CWepon mWepon;//����̃C���X�^���X

	CCollider mColSphereHead; //��
	CCollider mColSphereLeg;  //��
	CColliderCapsule mColBody;//�L�����ƃL�������d�Ȃ�Ȃ����߂̑̃R���C�_
	CCollider mColSphereSearch;//�v���C���[���G�p�R���C�_

	static CModelX sModel;//���f���f�[�^

	bool IsFoundPlayer;//�v���C���[�������Ă���
	bool IsInRange;    //�˒����Ƀv���C���[������
};
#endif