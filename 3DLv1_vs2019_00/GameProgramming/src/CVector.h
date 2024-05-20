
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
	
	/// <summary>
	/// �_�Ɛ�(�n�_�A�I�_��ʂ钼��)�̍ŒZ���������߂�
	/// </summary>
	/// <param name="p">�_</param>
	/// <param name="s">�n�_</param>
	/// <param name="e">�I�_</param>
	/// <param name="mp">���̍ŒZ�_</param>
	/// <param name="t">����</param>
	/// <returns>�����̒���</returns>
	float CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t);
	/// <summary>
	/// 2���Ԃ̍ŒZ������Ԃ�
	/// </summary>
	/// <param name="s1">�n�_1</param>
	/// <param name="e1">�I�_1</param>
	/// <param name="s2">�n�_2</param>
	/// <param name="e2">�I�_2</param>
	/// <param name="mp1">��_1</param>
	/// <param name="mp2">��_2</param>
	/// <param name="t1">�䗦1</param>
	/// <param name="t2">�䗦2</param>
	/// <returns>2���Ԃ̍ŒZ����</returns>
	float CalcLineLineDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
		CVector* mp1, CVector* mp2, float* t1, float* t2);
	//0~1�̊ԂɃN�����v(�l�������I�ɔ͈͓��ɂ���)
	void Clamp0to1(float& v);
	/// <summary>
	/// 2�����Ԃ̍ŒZ������Ԃ�
	/// </summary>
	/// <param name="s1">�n�_1</param>
	/// <param name="e1">�I�_1</param>
	/// <param name="s2">�n�_2</param>
	/// <param name="e2">�I�_2</param>
	/// <param name="mp1">�ŒZ���̒[�_1</param>
	/// <param name="mp2">�ŒZ���̒[�_2</param>
	/// <returns>2�����Ԃ̍ŒZ����</returns>
	float CalcSegmentSegmentDist(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2,
		CVector* mp1, CVector* mp2);
		

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
	CVector operator*(const CMatrix& m)const;
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