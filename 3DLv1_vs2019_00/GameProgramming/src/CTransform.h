#ifndef CTRANSFORM_H
#define CTRANSFORM_H
#include "CVector.h"
/*
�g�����X�t�H�[���N���X
�ʒu�A��]�A�g�k�A�ϊ��s��̊�{�N���X
*/
class CTransform
{
public:
	//�ʒu�̐ݒ�
    //SetPosition(�ʒu)
	void SetPosition(const CVector& v);
	//�ʒu�̎擾
	const CVector& GetPosition() const;
	//��]�l�̐ݒ�
	//SetRotation(��]�l);
	void SetRotation(const CVector& v);
	//�g��k���̐ݒ�
	//SetScale(�g��k��)
	void SetScale(const CVector& v);

	//�����s��̎擾
	const CMatrix& GetMatrix() const;
	//��]�s��̎擾
	const CMatrix& GetMatrixRotate() const;
	//�s��X�V����
	void Update();
	//Update(�ʒu,��],�X�P�[��)
	void Update(const CVector& pos, const CVector& rot, const CVector& scale);
protected://�q�N���X�̓A�N�Z�X�\
	CVector mPosition;//�ʒu
	CVector mRotation;//��]
	CVector mScale;//�g��k��
	CMatrix mMatrixTranslate;//���s�ړ��s��
	CMatrix mMatrixRotate;//��]�s��
	CMatrix mMatrixScale;//�g��k���s��
	CMatrix mMatrix;//�����s��
};
#endif