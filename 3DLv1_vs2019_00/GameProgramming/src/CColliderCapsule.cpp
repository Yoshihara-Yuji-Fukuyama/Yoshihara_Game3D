#include "CColliderCapsule.h"

CColliderCapsule::CColliderCapsule(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius)
{
	Set(parent, matrix, v0, v1, radius);
}

//�J�v�Z���R���C�_�̐ݒ�
void CColliderCapsule::Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius)
{
	mType = EType::ECAPSULE;
	mpParent = parent;
	mpMatrix = matrix;
	mSp = v0;
	mEp = v1;
	mRadius = radius;
}

//���W�̍X�V
void CColliderCapsule::Update()
{
	//���[���h���W�ɍX�V���A���̌��������߂�
	mV[0] = mSp * *mpMatrix;
	mV[1] = mEp * *mpMatrix;
	mV[2] = mV[1] - mV[0];
}

//�`��
void CColliderCapsule::Render()
{
	//�A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	//�u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//���C�g�I�t
	glDisable(GL_LIGHTING);
	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,0.4f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	//�s��ޔ�
	glPushMatrix();

	//�n�_
	//�s��ޔ�
	glPushMatrix();
	CVector center;
	center = mV[0];// -(v1 - v0).Normalize() * mRadius;
	glTranslatef(center.GetX(), center.GetY(), center.GetZ());
	//���`��
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();

	//�I�_
	//�s��ޔ�
	glPushMatrix();
	center = mV[1];// +(v1 - v0).Normalize() * mRadius;
	glTranslatef(center.GetX(), center.GetY(), center.GetZ());
	//���`��
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();

	//�~���`��
	CVector line = (mV[1] - mV[0]);
	center = mV[0] + line * 0.5f;
	CVector lineXZ = line;
	lineXZ.SetY(0.0f);
	if (lineXZ.Length() > 0.0f)
	{
		lineXZ = lineXZ.Normalize();
	}
	line = line.Normalize();
	float lineLength = (mV[1] - mV[0]).Length();// -mRadius * 2;
	glTranslatef(center.GetX(), center.GetY(), center.GetZ());
	//rotate Y-axis
	CMatrix rot;
	if (lineXZ.Length() > 0.001)
	{
		rot.GetM()[0] = rot.GetM()[4 * 2 + 2] = lineXZ.GetZ();
		rot.GetM()[2] = -lineXZ.GetX();
		rot.GetM()[4 * 2] = lineXZ.GetX();
	}
	glMultMatrixf(rot.GetM());

	//rotate X-axis
	rot.SetIdentity();
	float cos = sqrtf(line.GetX() * line.GetX() + line.GetZ() * line.GetZ());
	rot.GetM()[4 + 1] = rot.GetM()[8 + 2] = cos;
	rot.GetM()[4 + 2] = -line.GetY();
	rot.GetM()[8 + 1] = line.GetY();
	glMultMatrixf(rot.GetM());

	glTranslatef(0.0f, 0.0f, -lineLength / 2.0f);
	GLUquadric* mesh;
	mesh = gluNewQuadric();
	gluCylinder(mesh, mRadius, mRadius, lineLength, 16, 16);
	//���C�g�I��
	glEnable(GL_LIGHTING);
	//�A���t�@�u�����h����
	glDisable(GL_ALPHA);
	//�s�񕜋A
	glPopMatrix();
}

//�D�揇�ʂ̍X�V
void CColliderCapsule::ChangePriority()
{
	//mV[0]��mV[1]�̒��S�����߂�
	CVector pos = (mV[0] + mV[1]) * (0.5f);
	//�x�N�g���̒������D��x
	CCollider::ChangePriority(pos.Length());
}
