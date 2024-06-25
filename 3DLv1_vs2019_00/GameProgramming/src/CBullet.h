#ifndef CBULLET_H
#define CBULLET_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
//�O�p�`�N���X�̃C���N���[�h
#include "CTriangle.h"
//�R���C�_�N���X�̃C���N���[�h
#include "CCollider.h"

/*
�e�N���X
�O�p�`���΂�
*/
class CBullet : public CCharacter3
{
public:
	//�e��ݒ�
	CBullet(CCharacter3* parent);
	//�傫���̐ݒ�
	void SetScale(float scale);
	//�X�V
	void Update();

	//�Փˏ���
	void Collision();
	//�Փˏ���
	//Collision(�R���C�_1,�R���C�_2)
	void Collision(CCollider* m, CCollider* o);
	//����������ݒ�
	void SetDirection(CVector m, CVector o);
private:
	int mLife;//��������
	CVector mMoveDirection;//��������
	CCollider mCollider;//�R���C�_
	static CModel sModel;//���f���f�[�^
};
#endif