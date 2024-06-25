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
	//�L�����^�O���
	enum class ECharaTag
	{
		EPLAYER,//0
		EENEMY, //1
		EWEPON, //2
	};
	//�����̃L�����^�O��Ԃ�
	ECharaTag GetCharaTag();

	CCharacter3();
	CCharacter3(int priority);
	~CCharacter3();

	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}
	//���f���̐ݒ�
	//Model(CModel*m);
	void Model(CModel* m);
	//�e�|�C���^�̎擾
	CCharacter3* GetParent();
	//�`�揈��
	void Render();
	/// <summary>
	/// �L�����̕��������������������ɕς���
	/// </summary>
	/// <param name="charZ">�L�����̕���</param>
	/// <param name="direction">��������������</param>
	void ChangeDirection(CVector charZ, CVector direction, float margin = 0.06f);
protected:
	CModel* mpModel;//���f���̃|�C���^
	CCharacter3* mpParent;//�e
	ECharaTag mCharaTag;   //�����̃L�����^�O
};
#endif