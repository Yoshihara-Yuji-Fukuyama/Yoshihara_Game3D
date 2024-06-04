#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CActionCamera.h"
#include "CWepon.h"

class CXPlayer : public CXCharacter
{
public:
	CXPlayer();
	//�C���X�^���X�̎擾
	static CXPlayer* GetInstance();

	//�X�V
	//�ړ��A�U������
	//�ϊ��s��A�A�j���[�V�����A�J�v�Z���R���C�_�̍X�V
	void Update();
	//����̍X�V
	void WeponUpdate();
	//����̕`��
	void WeponRender();
	//�}�E�X�ړ���Y����]
	//turnHorizontal:���̉�]��
	void Turn(float turnHorizontal);
	//�Փ˔���
	void Collision(CCollider* m, CCollider* o);
	//�����ݒ�
	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	void Init(CModelX* model);
private:
	static CXPlayer* spInstance;//�C���X�^���X
	CCollider mColSphereHead; //��
	CCollider mColSphereBody; //��
	//CColliderCapsule mColBody;//�L�����ƃL�������d�Ȃ�Ȃ����߂̑̃R���C�_
	CCollider mColSphereSword;//��
	CInput mInput;//CInput�̃C���X�^���X�쐬
	//�E��]�A����]���������ǂ���
	bool IsLeftTurn;
	bool IsRightTurn;
	bool IsGround;//�n�ʂɂ��Ă��邩
	bool IsJump;//�W�����v���Ă��邩�ǂ���
	float mJumpPower;//�W�����v��
	CWepon mWepon;
};
#endif