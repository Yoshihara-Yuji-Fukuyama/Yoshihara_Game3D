#include "CMatrix.h"
//�W�����o�͊֐��̃C���N���[�h
#include <stdio.h>
//�~����M_PI��L���ɂ���
#define _USE_MATH_DEFINES
//���w�֐��̃C���N���[�h
#include <math.h>
#include "CVector.h"

void CMatrix::Print()
{
	printf("%10f %10f %10f %10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f %10f %10f %10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f %10f %10f %10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f %10f %10f %10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

CMatrix::CMatrix()
{
	Identity();
}

//�P�ʍs��̍쐬
CMatrix CMatrix::Identity()
{
	mM[0][0] = 1.0f; mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	mM[1][0] = 0.0f; mM[1][1] = 1.0f; mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = 1.0f; mM[2][3] = 0.0f;
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;
	//���̍s���Ԃ�
	return *this;
}

//�g��k���s��̍쐬
//Scale(�{��X,�{��Y,�{��Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz)
{
	mM[0][0] = sx;   mM[0][1] = 0.0f; mM[0][2] = 0.0f; mM[0][3] = 0.0f;
	mM[1][0] = 0.0f; mM[1][1] = sy;   mM[1][2] = 0.0f; mM[1][3] = 0.0f;
	mM[2][0] = 0.0f; mM[2][1] = 0.0f; mM[2][2] = sz;   mM[2][3] = 0.0f;
	mM[3][0] = 0.0f; mM[3][1] = 0.0f; mM[3][2] = 0.0f; mM[3][3] = 1.0f;
	//���̍s���Ԃ�
	return *this;
}

float CMatrix::M(int r, int c)const
{
	return mM[r][c];
}

//��]�s��(Y��)�̍쐬
//RotateY(�p�x)
CMatrix CMatrix::RotateY(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Y���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//�s���Ԃ�
	return *this;
}

//��]�s��(Z��)�̍쐬
//RotateZ(�p�x)
CMatrix CMatrix::RotateZ(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Z���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//�s���Ԃ�
	return *this;
}

//��]�s��(X��)�̍쐬
//RotateX(�p�x)
CMatrix CMatrix::RotateX(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//X���ŉ�]����s��̐ݒ�
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//�s���Ԃ�
	return *this;
}

//�ړ��s��̍쐬
//Translate(�ړ���X,�ړ���Y,�ړ���Z)
CMatrix CMatrix::Translate(float mx, float my, float mz)
{
	//�P�ʍs��ɂ���
	Identity();
	//�ړ��ʂ̐ݒ�
	mM[3][0] = mx;
	mM[3][1] = my;
	mM[3][2] = mz;
	//���̍s���Ԃ�
	return *this;
}

void CMatrix::M(int row, int col, float value)
{
	mM[row][col] = value;
}

//*���Z�q�̃I�[�o�[���[�h
//CMatrix * CMatrix�̉��Z���ʂ�Ԃ�
const CMatrix CMatrix::operator*(const CMatrix& m)const
{
	CMatrix t;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			t.mM[j][i] = mM[j][0] * m.mM[0][i] + mM[j][1] * m.mM[1][i] + mM[j][2] * m.mM[2][i] + mM[j][3] * m.mM[3][i];
		}
	}

	return t;
}

float* CMatrix::M() const
{
	return (float*)mM[0];
}

CMatrix CMatrix::Transpose() const
{
	CMatrix tmp;//�ԋp�p��CMatrix�C���X�^���X���쐬

	//tmp�̓񎟌��z��ɁA�t�s�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//2�����z���j�si��ڂ̒l��������
			tmp.mM[i][j] = mM[j][i];
		}
	}
	return tmp;//������ꂽtmp��Ԃ�
}

CVector CMatrix::VectorZ() const
{
	return CVector(mM[2][0], mM[2][1], mM[2][2]);
}

CVector CMatrix::VectorX() const
{
	return CVector(mM[0][0], mM[0][1], mM[0][2]);
}

CVector CMatrix::VectorY() const
{
	return CVector(mM[1][0], mM[1][1], mM[1][2]);
}


int CMatrix::Size()
{
	return sizeof(mM) / sizeof(float);
}

CMatrix CMatrix::Quaternion(float x, float y, float z, float w)
{
	mM[0][0] = x * x - y * y - z * z + w * w;
	mM[0][1] = 2 * x * y - 2 * w * z;
	mM[0][2] = 2 * x * z + 2 * w * y;
	mM[0][3] = 0;
	mM[1][0] = 2 * x * y + 2 * w * z;
	mM[1][1] = -x * x + y * y - z * z + w * w;
	mM[1][2] = 2 * y * z - 2 * w * x;
	mM[1][3] = 0;
	mM[2][0] = 2 * x * z - 2 * w * y;
	mM[2][1] = 2 * y * z + 2 * w * x;
	mM[2][2] = -x * x - y * y + z * z + w * w;
	mM[2][3] = 0;
	mM[3][0] = 0;
	mM[3][1] = 0;
	mM[3][2] = 0;
	mM[3][3] = 1;
	return *this;
}

CMatrix CMatrix::operator*(const float& x) const
{
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t.mM[i][j] = mM[i][j] * x;
		}
	}
	return  t;
}

CMatrix CMatrix::operator+(const CMatrix& m) const
{
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t.mM[i][j] = mM[i][j] + m.mM[i][j];
		}
	}
	return t;
}

void CMatrix::operator+=(const CMatrix& m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mM[i][j] += m.mM[i][j];
		}
	}
}
