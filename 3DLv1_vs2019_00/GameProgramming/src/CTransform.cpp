#include "CTransform.h"

const CVector& CTransform::GetPosition() const
{
	return mPosition;
}

void CTransform::SetPosition(const CVector& v)
{
	mPosition = v;
}

void CTransform::SetRotation(const CVector& v)
{
	mRotation = v;
}

void CTransform::SetScale(const CVector& v)
{
	mScale = v;
}

const CMatrix& CTransform::GetMatrix() const
{
	return mMatrix;
}

const CMatrix& CTransform::GetMatrixRotate() const
{
	return mMatrixRotate;
}

void CTransform::Update(const CVector& pos, const CVector& rot, const CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	Update();
}

//�s��X�V����
void CTransform::Update()
{
	//�g��k���s��̐ݒ�
	mMatrixScale.SetScale(mScale.GetX(), mScale.GetY(), mScale.GetZ());
	//��]�s��̐ݒ�
	mMatrixRotate = 
		CMatrix().SetRotateZ(mRotation.GetZ()) *
		CMatrix().SetRotateX(mRotation.GetX()) * 
		CMatrix().SetRotateY(mRotation.GetY());
	//���s�ړ��s��̐ݒ�
	mMatrixTranslate.SetTranlate(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());
	//�����s��̐ݒ�
	mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;
}