
#ifndef CVECTOR_H
#define CVECTOR_H
#include "CMatrix.h"

/*
�x�N�g���N���X
�x�N�g���f�[�^�������܂�
*/
class CVector 
{
public:
	CVector();
	//�R���X�g���N�^
	//CVector(X���W,Y���W,Z���W)
	CVector(float x, float y, float z);

	//���K��
	//�傫��1�̃x�N�g����Ԃ�
	CVector Normalize() const;
	//�O��
	//Cross(�x�N�g��)
	CVector Cross(const CVector& v)const;
	//����
	//Dot(�x�N�g��)
	float Dot(const CVector& v)const;
	//�x�N�g���̒�����Ԃ�
	float Length() const;

	//+���Z�q�̃I�[�o�[���[�h
    //CVector + CVector�̉��Z���ʂ�Ԃ�
	CVector operator+(const CVector& v)const;
	//-���Z�q�̃I�[�o�[���[�h
	//CVector - CVector�̉��Z���ʂ�Ԃ�
	CVector operator-(const CVector& v)const;
	//*���Z�q�̃I�[�o�[���[�h
    //CVector * float�̉��Z���ʂ�Ԃ�
	CVector operator*(const float& f)const;
	//CVector * CMatrix�̌��ʂ�CVector�ŕԂ�
	CVector operator*(const CMatrix& m);
	//+=���Z�q�̃I�[�o�[���[�h
	//CVector1 += CVector2�̉��Z���s��
	void operator+=(const CVector& v);

	//�e���ł̒l�̐ݒ�
	//Set(X���W, Y���W, Z���W)
	void Set(float x, float y, float z);
	//X�̒l��ݒ�
	void SetX(float f);
	//Y�̒l��ݒ�
	void SetY(float f);
	//Z�̒l��ݒ�
	void SetZ(float f);
	//X�̒l��Ԃ�
	float GetX() const;
	//Y�̒l��Ԃ�
	float GetY() const;
	//Z�̒l��Ԃ�
	float GetZ() const;
private:
	//3D�e���ł̒l��ݒ�
	float mX, mY, mZ;
};
#endif