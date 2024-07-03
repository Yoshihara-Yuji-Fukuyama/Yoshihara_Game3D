#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CWepon.h"
#include "CEnemyAi.h"

class CXEnemy : public CXCharacter , public CEnemyAi
{
public:
	CXEnemy();
	CXEnemy(CVector pos);	//���W��ݒ�
	void Init(CModelX* model);	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	~CXEnemy();	//�G�̐��������̐��������炷

	void Update();	//�X�V

	void WeponUpdate();	//����̍X�V
	void WeponRender();	//����̕`��
	
	void Collision(CCollider* m, CCollider* o);	//�Փˏ���

	//�s���n
	void Wait();      //�ҋ@
	void Wandering(); //�p�j
	void Chase();     //�ǐ�
	void Attack();    //�U��
	void MoveAttack();//�����Ȃ���U��
	void Reload();    //�����[�h
	void Escape();    //���S
	void Damage();    //��e
	void Die();       //���S
private:
	CWepon mWepon;//����̃C���X�^���X

	CCollider mColSphereHead; //��
	CCollider mColSphereLeg;  //��
	CColliderCapsule mColBody;//�L�����ƃL�������d�Ȃ�Ȃ����߂̑̃R���C�_
	CCollider mColSphereSearch;//�v���C���[���G�p�R���C�_

	static CModelX sModel;	//���f���f�[�^

	bool IsFoundPlayer;//�v���C���[�������Ă���
	bool IsInRange;    //�˒����Ƀv���C���[������
};
#endif