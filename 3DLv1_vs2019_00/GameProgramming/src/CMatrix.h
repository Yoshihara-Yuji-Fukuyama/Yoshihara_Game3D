#ifndef CMATRIX_H
#define CMATRIX_H

class CVector;
/*
�}�g���N�X�N���X
4�s4��̍s��f�[�^�������܂�
*/

class CMatrix
{
public:
	CVector VectorZ() const; //Z���x�N�g���̎擾
	CVector VectorX() const; //X���x�N�g���̎擾
	CVector VectorY() const; //Y���x�N�g���̎擾
	//�t�s��擾
	CMatrix Transpose() const;
	//�s��̎擾
	float* M() const;
	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix * CMatrix�̉��Z���ʂ�Ԃ�
	const CMatrix operator*(const CMatrix& m)const;
	//�s��l�̑��
	//M(�s��,��,�l)
	void M(int row, int col, float value);
	//�ړ��s��̍쐬
	//Translate(�ړ���X,�ړ���Y,�ړ���Z)
	CMatrix Translate(float mx, float my, float mz);
	//��]�s��(X��)�̍쐬
	//RotateX(�p�x)
	CMatrix RotateX(float degree);
	//��]�s��(Z��)�̍쐬
	//RotateZ(�p�x)
	CMatrix RotateZ(float degree);
	//��]�s��(Y��)�̍쐬
	//RotateY(�p�x)
	CMatrix RotateY(float degree);
	//�s��l�̎擾
	//M(�s,��)
	//mM[�s][��]���擾
	float M(int r, int c)const;
	//�g��k���s��̍쐬
	//Scale(�{��X,�{��Y,�{��Z)
	CMatrix Scale(float sx, float sy, float sz);
	//�\���m�F�p
	//4�~4�̍s�����ʏo��
	void Print();
	//�f�t�H���g�R���X�g���N�^
	CMatrix();
	//�P�ʍs��̍쐬
	CMatrix Identity();
	//�v�f���̎擾
	int Size();
	//�N�I�[�^�j�I���ŉ�]�s���ݒ肷��
	CMatrix Quaternion(float x, float y, float z, float w);
	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix *float�̉��Z���ʂ�Ԃ�
	CMatrix operator*(const float& x)const;
	//+���Z�q�̃I�[�o�[���[�h
	//CMatrix1 + CMatrix2 �̉��Z���ʂ�Ԃ�
	CMatrix operator+(const CMatrix& m)const;
	//+=���Z�q�̃I�[�o�[���[�h
	//CMatrix1 += CMatrix2�̉��Z���s��
	void operator+=(const CMatrix& m);
private:
	//4�~4�̍s��f�[�^��ݒ�
	float mM[4][4];
};
#endif
