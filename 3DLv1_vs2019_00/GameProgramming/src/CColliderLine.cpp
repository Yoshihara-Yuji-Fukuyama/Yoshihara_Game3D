#include "CColliderLine.h"

CColliderLine::CColliderLine(CCharacter3* parent, CMatrix* matrix
	, const CVector& v0, const CVector& v1)
{
	Set(parent, matrix, v0, v1);
}

void CColliderLine::Set(CCharacter3* parent, CMatrix* matrix
	, const CVector& v0, const CVector& v1)
{
	mType = EType::ELINE;//�����R���C�_
	mpParent = parent;//�e�ݒ�
	if (matrix)
		mpMatrix = matrix;//�e�s�񂠂�ΐݒ�
	//�������_�ݒ�
	mV[0] = v0;
	mV[1] = v1;
}

void CColliderLine::Render()
{
	//�s��ޔ�
	glPushMatrix();
	//�s��K�p
	glMultMatrixf(mpMatrix->GetM());

	//���u�����h��L���ɂ���
	glEnable(GL_BLEND);
	//�u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//���C�g�I�t
	glDisable(GL_LIGHTING);

	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	//�����`��
	glBegin(GL_LINES);
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glEnd();

	//���C�g�I��
	glEnable(GL_LIGHTING);
	//���u�����h�𖳌�
	glDisable(GL_ALPHA);
	//�s�񕜋A
	glPopMatrix();
}

//�D��x�̕ύX
void CColliderLine::ChangePriority()
{
	//mV[0]��mv[1]�̒��S�����߂�
	CVector pos = (mV[0] * *mpMatrix + mV[1] * *mpMatrix) * (0.5f);
	//�x�N�g���̒������D��x
	CCollider::ChangePriority(pos.Length());
}