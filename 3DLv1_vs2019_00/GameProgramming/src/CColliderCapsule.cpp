#include "CColliderCapsule.h"

CColliderCapsule::CColliderCapsule(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius)
{
	Set(parent, matrix, v0, v1, radius);
}

//カプセルコライダの設定
void CColliderCapsule::Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius)
{
	mType = EType::ECAPSULE;
	mpParent = parent;
	mpMatrix = matrix;
	mSp = v0;
	mEp = v1;
	mRadius = radius;
}

//座標の更新
void CColliderCapsule::Update()
{
	//ワールド座標に更新し、線の向きを求める
	mV[0] = mSp * *mpMatrix;
	mV[1] = mEp * *mpMatrix;
	mV[2] = mV[1] - mV[0];
}

//描画
void CColliderCapsule::Render()
{
	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ライトオフ
	glDisable(GL_LIGHTING);
	//DIFFUSE赤色設定
	float c[] = { 1.0f,0.0f,0.0f,0.4f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	//行列退避
	glPushMatrix();

	//始点
	//行列退避
	glPushMatrix();
	CVector center;
	center = mV[0];// -(v1 - v0).Normalize() * mRadius;
	glTranslatef(center.GetX(), center.GetY(), center.GetZ());
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();

	//終点
	//行列退避
	glPushMatrix();
	center = mV[1];// +(v1 - v0).Normalize() * mRadius;
	glTranslatef(center.GetX(), center.GetY(), center.GetZ());
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();

	//円柱描画
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
	//ライトオン
	glEnable(GL_LIGHTING);
	//アルファブレンド無効
	glDisable(GL_ALPHA);
	//行列復帰
	glPopMatrix();
}

//優先順位の更新
void CColliderCapsule::ChangePriority()
{
	//mV[0]とmV[1]の中心を求める
	CVector pos = (mV[0] + mV[1]) * (0.5f);
	//ベクトルの長さが優先度
	CCollider::ChangePriority(pos.Length());
}
