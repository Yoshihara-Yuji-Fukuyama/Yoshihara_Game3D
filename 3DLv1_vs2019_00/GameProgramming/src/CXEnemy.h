#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CWepon.h"
#include "CEnemyAi.h"
#include <time.h>

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

	bool IsTime(int lag);//lag�b���Ƃ�true��Ԃ�

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
	CCollider mColSphereBody; //��
	CCollider mColSphereLeg;  //��
	CColliderCapsule mColBody;//�L�����ƃL�������d�Ȃ�Ȃ����߂̑̃R���C�_
	CCollider mColSphereSearch;//�v���C���[���G�p�R���C�_

	clock_t start;//�n�܂�̎��Ԃ�ۑ�
	double mCount;//�b���J�E���^


	static CModelX sModel;	//���f���f�[�^

	bool IsFoundPlayer;//�v���C���[�������Ă���
	bool IsInRange;    //�˒����Ƀv���C���[������
	bool IsAttack;     //�U���ł��邩
};
#endif