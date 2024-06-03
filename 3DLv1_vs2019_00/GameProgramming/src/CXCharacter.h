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
	/// <summary>
	/// �L�����̕��������������������ɕς���
	/// </summary>
	/// <param name="charZ">�L�����̕���</param>
	/// <param name="direction">��������������</param>
	void ChangeDirection(CVector charZ,CVector direction);
protected:
	CModelX* mpModel;//���f���f�[�^
	int mAnimationIndex;      //�A�j���[�V�����ԍ�
	bool mAnimationLoopFlg;   //true:�A�j���[�V�������J��Ԃ�
	float mAnimationFrame;    //�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;//�A�j���[�V�����̍Đ��t���[����
	CMatrix* mpCombinedMatrix;//�����s��ޔ�
};
#endif