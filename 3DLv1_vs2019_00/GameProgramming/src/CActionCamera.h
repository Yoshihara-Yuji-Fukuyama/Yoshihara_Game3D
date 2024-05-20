#ifndef CACTIONCAMERA_H
#define CACTIONCAMERA_H

#include<GLFW/glfw3.h>
#include "CVector.h"
#include "CTransform.h"
#include "CMatrix.h"
#include "CInput.h"

//�J�����N���X
class CActionCamera : public CTransform
{
public:
	CActionCamera();
	//�C���X�^���X�̎擾
	static CActionCamera* GetInstance();

	/// <summary>
	/// �ݒ胁�\�b�h
	/// </summary>
	/// <param name="distance">����</param>
	/// <param name="xaxis">X��]</param>
	/// <param name="yaxis">Y��]</param>
	void Set(float distance, float xaxis, float yaxis);
	//�J�����X�V
	void Update();
	//�J�����K�p
	void Render();
	//�}�E�X�̈ړ��̃R�[���o�b�N�֐�
	//�}�E�X���ړ�����ƑO�̍��W�Ƃ̍�����ݒ肷��
	void MouseCallback(double xpos,double ypos);
	//�J������X���擾
	CVector GetVectorX();
	//�J������Z���擾
	CVector GetVectorZ();
	/// <summary>
	/// ���[���h���W���X�N���[�����W�֕ϊ�����
	/// </summary>
	/// <param name="screen">�X�N���[�����W</param>
	/// <param name="world">���[���h���W</param>
	/// <returns></returns>
	bool WorldToScreen(CVector* screen, const CVector& world);
private:
	static CActionCamera* spInstance;//�C���X�^���X
	CVector mEye;	//���_�̈ʒu
	CVector mCenter;//�����_
	CVector mUp;	//�����
	CMatrix mModelView;//���f���r���[�s��
	CInput mInput;//���̓N���X
	int mScreenWidth; //��
	int mScreenHeight;//����
	CMatrix mProjection;//�v���W�F�N�V�����s��
	bool mFirstMouse;//�}�E�X�������̂����߂Ă��ǂ���
	float mLastX;//�O�̃}�E�XX���W
	float mLastY;//�O�̃}�E�XY���W
	float mTurnVertical;//�c�̉�]��
	float mTurnHorizontal;//���̉�]��
	float mSensitivity;//�}�E�X���x
};
#endif