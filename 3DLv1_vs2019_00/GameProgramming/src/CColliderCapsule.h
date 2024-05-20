#ifndef CCOLLIDERCAPSULE_H
#define CCOLLIDERCAPSULE_H

#include "CCollider.h"
//�J�v�Z���R���C�_�N���X
class CColliderCapsule : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="radius">���a</param>
	CColliderCapsule(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	/// <summary>
	/// �J�v�Z���R���C�_�̐ݒ�
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="radius">���a</param>
	void Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	//���W�̍X�V
	void Update();
	//�`��
	void Render();
	//�D�揇�ʂ̍X�V
	void ChangePriority();
private:
	CVector mSp;//�n�_
	CVector mEp;//�I�_
};
#endif
