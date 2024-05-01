#include "CTriangle.h"
#include "glut.h"

//Normal(�@���x�N�g��1,�@���x�N�g��2,�@���x�N�g��3)
void CTriangle::Normal(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mN[0] = v0;
	mN[1] = v1;
	mN[2] = v2;
}
//���_���W�ݒ�
//Vertex(���_1,���_2,���_3)
void CTriangle::Vertex(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

//�@���ݒ�
//Normal(�@���x�N�g��)
void CTriangle::Normal(const CVector& n)
{
	mN[0] = mN[1] = mN[2] = n;
}

//�`��
void CTriangle::Render()
{
	glBegin(GL_TRIANGLES);
	glNormal3f(mN[0].GetX(), mN[0].GetY(), mN[0].GetZ());
	glTexCoord2f(mUv[0].GetX(), mUv[0].GetY());
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glNormal3f(mN[1].GetX(), mN[1].GetY(), mN[1].GetZ());
	glTexCoord2f(mUv[1].GetX(), mUv[1].GetY());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glNormal3f(mN[2].GetX(), mN[2].GetY(), mN[2].GetZ());
	glTexCoord2f(mUv[2].GetX(), mUv[2].GetY());
	glVertex3f(mV[2].GetX(), mV[2].GetY(), mV[2].GetZ());
	glEnd();
}

int CTriangle::MaterialIdx()
{
	return mMaterialIdx;
}

void CTriangle::MaterialIdx(int idx)
{
	mMaterialIdx = idx;
}

void CTriangle::UV(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mUv[0] = v0;
	mUv[1] = v1;
	mUv[2] = v2;
}

//�`��
//Render(�s��)
void CTriangle::Render(const CMatrix& m)
{
	CVector mV[3], mN[3];
	mV[0] = CTriangle::mV[0] * m;
	mV[1] = CTriangle::mV[1] * m;
	mV[2] = CTriangle::mV[2] * m;
	//�@���͈ړ��l��0
	CMatrix mat = m;
	mat.M(3, 0, 0.0f);
	mat.M(3, 1, 0.0f);
	mat.M(3, 2, 0.0f);
	mN[0] = CTriangle::mN[0] * mat;
	mN[1] = CTriangle::mN[1] * mat;
	mN[2] = CTriangle::mN[2] * mat;
	glBegin(GL_TRIANGLES);
	glNormal3f(mN[0].GetX(), mN[0].GetY(), mN[0].GetZ());
	glTexCoord2f(mUv[0].GetX(), mUv[0].GetY());
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glNormal3f(mN[1].GetX(), mN[1].GetY(), mN[1].GetZ());
	glTexCoord2f(mUv[1].GetX(), mUv[1].GetY());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glNormal3f(mN[2].GetX(), mN[2].GetY(), mN[2].GetZ());
	glTexCoord2f(mUv[2].GetX(), mUv[2].GetY());
	glVertex3f(mV[2].GetX(), mV[2].GetY(), mV[2].GetZ());
	glEnd();
}

//���_���W�𓾂�
const CVector& CTriangle::V0() const //mV[0]��Ԃ��܂�
{
	return mV[0];
}
const CVector& CTriangle::V1() const //mV[1]��Ԃ��܂�
{
	return mV[1];
}
const CVector& CTriangle::V2() const //mV[2]��Ԃ��܂�
{
	return mV[2];
}
//�@���𓾂�
const CVector& CTriangle::N0() const //mN[0]��Ԃ��܂�
{
	return mN[0];
}
const CVector& CTriangle::N1() const //mN[1]��Ԃ��܂�
{
	return mN[1];
}
const CVector& CTriangle::N2() const //mN[2]��Ԃ��܂�
{
	return mN[2];
}
//UV�𓾂�
const CVector& CTriangle::U0() const //��Uv[0]��Ԃ��܂�
{
	return mUv[0];
}
const CVector& CTriangle::U1() const //��Uv[1]��Ԃ��܂�
{
	return mUv[1];
}
const CVector& CTriangle::U2() const //��Uv[2]��Ԃ��܂�
{
	return mUv[2];
}