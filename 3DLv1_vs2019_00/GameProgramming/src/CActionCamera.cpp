#include "CActionCamera.h"
#include "glut.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "CXPlayer.h"

CActionCamera* CActionCamera::spInstance = nullptr;

CActionCamera::CActionCamera()
	: mFirstMouse(true)
	, mLastX(400.0f)//�����}�E�X�ʒu�͒���
	, mLastY(300.0f)
	, mTurnVertical(0.0f)//������]�ʂ�0
	, mTurnHorizontal(0.0f)
	, mSensitivity(0.1f)//�������x
{
	spInstance = this;
}

//�C���X�^���X�̎擾
CActionCamera* CActionCamera::GetInstance()
{
	return spInstance;
}

//�ݒ胁�\�b�h
void CActionCamera::Set(float distance, float xaxis, float yaxis)
{
	SetRotation(CVector(xaxis, yaxis, 0.0f));
	SetScale(CVector(0.0f, 0.0f, distance));
	mUp = CVector(0.0f, 1.0f, 0.0f);
	spInstance = this;

	int viewport[4];
	//���݂̃r���[�|�[�g��ۑ����Ă���
	glGetIntegerv(GL_VIEWPORT, viewport);
	mScreenWidth = viewport[2]; //�����擾
	mScreenHeight = viewport[3];//�������擾
	//�v���W�F�N�V�����s��̎擾
	glGetFloatv(GL_PROJECTION_MATRIX, mProjection.GetM());
}

//�}�E�X�̈ړ��̃R�[���o�b�N�֐�
//�}�E�X���ړ�����ƑO�̍��W�Ƃ̍�����ݒ肷��
void CActionCamera::MouseCallback(double xpos, double ypos)
{
	//�}�E�X���삪���߂ĂȂ�
	if (mFirstMouse)
	{
		//�O�̍��W�ɍ��̍��W������
		mLastX = xpos;
		mLastY = ypos;
		mFirstMouse = false;
	}
	//X���W�̍�
	float xoffset = xpos - mLastX;
	//Y���W�̍�
	float yoffset = ypos - mLastY;
	//�O�̍��W���X�V����
	mLastX = xpos;
	mLastY = ypos;
	//���W�̍��Ɗ��x���|�������l����]�ʂɐݒ�
	mTurnVertical = yoffset * mSensitivity;//�c�̉�]��
	mTurnHorizontal = xoffset * mSensitivity;//���̉�]��
}

//�J�����X�V
void CActionCamera::Update()
{
	//��]�ʂ̕�������]������
	mRotation = mRotation - CVector(mTurnVertical, mTurnHorizontal, 0.0f);
	//CXPlayer::GetInstance()->mRotation;
	CTransform::Update();
	//�����_�̐ݒ�
	mCenter = mPosition;
	//���_�̈ʒu�̐ݒ�
	mEye = mPosition + mMatrixRotate.GetVectorZ() * mScale.GetZ();
	//��]�ʂ�0�ɂ���
	mTurnVertical = 0.0f;
	mTurnHorizontal = 0.0f;
}
//�J�����K�p
void CActionCamera::Render()
{
	gluLookAt(mEye.GetX(), mEye.GetY(), mEye.GetZ(),
		mCenter.GetX(), mCenter.GetY(), mCenter.GetZ(),
		mUp.GetX(), mUp.GetY(), mUp.GetZ());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelView.GetM());
}

//�J������X���擾
CVector CActionCamera::GetVectorX()
{
	return CVector(-mModelView.GetM(0, 0), -mModelView.GetM(1, 0), -mModelView.GetM(2, 0));
}
//�J������Z���擾
CVector CActionCamera::GetVectorZ()
{
	return CVector(-mModelView.GetM(0, 2), -mModelView.GetM(1, 2), -mModelView.GetM(2, 2));
}

//���[���h���W���X�N���[�����W�֕ϊ�����
bool CActionCamera::WorldToScreen(CVector* screen, const CVector& world)
{
	//���W�ϊ�
	CVector modelview_pos = world * mModelView;
	//��ʊO�Ȃ̂Ń��^�[��
	if (modelview_pos.GetZ() >= 0.0f)
	{
		return false;
	}

	//���W����
	CVector screen_pos = modelview_pos * mProjection;
	screen_pos = screen_pos * (1.0f / -modelview_pos.GetZ());

	//�X�N���[���ϊ�
	screen->SetX((1.0f + screen_pos.GetX()) * mScreenWidth * 0.5f);
	screen->SetY((1.0f + screen_pos.GetY()) * mScreenHeight * 0.5f);
	screen->SetZ(screen_pos.GetZ());

	return true;
}
