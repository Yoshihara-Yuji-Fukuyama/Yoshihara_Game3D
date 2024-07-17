#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMatrix.h"
#include "CCharacter3.h"

class CXCharacter : public CCharacter3 {
public:


	CXCharacter();
	virtual ~CXCharacter()
	{
		SAFE_DELETE_ARRAY(mpCombinedMatrix);
	}

	//�X�V����
	void CXCharacter::Update();
	//����������
	void Init(CModelX* model);
	/// <summary>
	/// �A�j���[�V������؂�ւ���
	/// </summary>
	/// <param name="index">�A�j���[�V�����ԍ�</param>
	/// <param name="loop">true�Ȃ�J��Ԃ�</param>
	/// <param name="framesize">�Ō�܂ōĐ�����̂Ɏg�p�����t���[����</param>
	void ChangeAnimation(int index, bool loop, float framesize);
	//�X�V����
	//matrix:�ړ��A��]�A�g��k���̍s��
	void Update(CMatrix& matrix);
	//�`�揈��
	void Render();
	//�A�j���[�V�����̍Đ��I������
	//true:�I���@false:�Đ���
	bool IsAnimationFinished();
	//�A�j���[�V�����ԍ��̎擾
	int GetAnimationIndex();

	//����ł��邩�ǂ���
	bool IsDead();
protected:
	CModelX* mpModel;//���f���f�[�^
	int mAnimationIndex;      //�A�j���[�V�����ԍ�
	bool mAnimationLoopFlg;   //true:�A�j���[�V�������J��Ԃ�
	float mAnimationFrame;    //�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;//�A�j���[�V�����̍Đ��t���[����
	CMatrix* mpCombinedMatrix;//�����s��ޔ�

	int mHp;//�̗�
	float mSpeed;//�v���C���[�̈ړ����x
	float mJumpPower;//�W�����v��

	bool IsDeath;//����ł��邩�ǂ���
	bool IsMove;//�����Ă��邩�ǂ���
	bool IsRun;//�����Ă��邩�ǂ���
	bool IsGround;//�n�ʂɂ��Ă��邩
	bool IsJump;//�W�����v���Ă��邩�ǂ���
	bool IsReloading;//�����[�h���Ă��邩
	bool IsWalkReload;//�����ă����[�h���Ă��邩
	bool IsWaitReload;//�~�܂��ă����[�h���Ă��邩
	bool IsHit;//��e���Ă��邩�ǂ���
	bool IsRoll;//���[�����O���Ă��邩
};
#endif