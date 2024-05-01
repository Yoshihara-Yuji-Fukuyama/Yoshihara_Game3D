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
	CMatrix();

	CVector GetVectorZ() const; //Z���x�N�g���̎擾
	CVector GetVectorX() const; //X���x�N�g���̎擾
	CVector GetVectorY() const; //Y���x�N�g���̎擾
	//�t�s��擾
	CMatrix GetTranspose() const;
	//�s��̎擾
	float* GetM() const;

	//�s��l�̑��
	//M(�s��,��,�l)
	void M(int row, int col, float value);
	//�ړ��s��̍쐬
	//SetTranlate(�ړ���X,�ړ���Y,�ړ���Z)
	CMatrix SetTranlate(float mx, float my, float mz);
	//��]�s��(X��)�̍쐬
	//SetRotateX(�p�x)
	CMatrix SetRotateX(float degree);
	//��]�s��(Z��)�̍쐬
	//SetRotateZ(�p�x)
	CMatrix SetRotateZ(float degree);
	//��]�s��(Y��)�̍쐬
	//SetRotateY(�p�x)
	CMatrix SetRotateY(float degree);
	//�s��l�̎擾
	//GetM(�s,��)
	//mM[�s][��]���擾
	float GetM(int r, int c)const;
	//�g��k���s��̍쐬
	//Scale(�{��X,�{��Y,�{��Z)
	CMatrix SetScale(float sx, float sy, float sz);
	//�\���m�F�p
	//4�~4�̍s�����ʏo��
	void Print();
	//�P�ʍs��̍쐬
	CMatrix SetIdentity();
	//�v�f���̎擾
	int GetSize();
	//�N�I�[�^�j�I���ŉ�]�s���ݒ肷��
	CMatrix Quaternion(float x, float y, float z, float w);

	//*���Z�q�̃I�[�o�[���[�h
    //CMatrix * CMatrix�̉��Z���ʂ�Ԃ�
	const CMatrix operator*(const CMatrix& m)const;
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
