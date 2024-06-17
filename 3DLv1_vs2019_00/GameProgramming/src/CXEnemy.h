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

	float mEnemySpeed;//�G�̈ړ����x
	float mJumpPower;//�W�����v��

	bool IsMove;//�����Ă��邩�ǂ���
	bool IsRun;//�����Ă��邩�ǂ���
	bool IsGround;//�n�ʂɂ��Ă��邩
	bool IsJump;//�W�����v���Ă��邩�ǂ���
	bool IsReloading;//�����[�h���Ă��邩
	bool IsWalkReload;//�����ă����[�h���Ă��邩
	bool IsWaitReload;//�~�܂��ă����[�h���Ă��邩

	bool IsFoundPlayer;//�v���C���[�������Ă���
	bool IsInRange;    //�˒����Ƀv���C���[������
};
#endif