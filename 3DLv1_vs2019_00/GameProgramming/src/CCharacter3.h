#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//�ϊ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
#include "CTask.h"
class CCollider;
/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/
class CCharacter3 :public CTransform ,public CTask
{
public:
	CCharacter3();
	CCharacter3(int priority);
	~CCharacter3();

	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}
	//���f���̐ݒ�
	//Model(CModel*m);
	void Model(CModel* m);
	//�`�揈��
	void Render();
protected:
	CModel* mpModel;//���f���̃|�C���^
};
#endif