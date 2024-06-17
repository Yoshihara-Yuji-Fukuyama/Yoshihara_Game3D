#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
class CCollisionManager;

/*
�R���C�_�N���X
�Փ˔���f�[�^
*/
class CCollider :public CTransform, public CTask
{
	friend CCollisionManager;
public:
	//�R���C�_�^�C�v
	enum class EType
	{
		ESPHERE,  //���R���C�_
		ETRIANGLE,//�O�p�R���C�_
		ELINE,    //�����R���C�_
		ECAPSULE,  //�J�v�Z���R���C�_
	};
	//mType�̒l��Ԃ�
	CCollider::EType GetType();
	//�^�O
	enum class ETag
	{
		EHEAD, //��
		EBODY, //��
		ELEG,  //��
		ESEARCH,//���G�p
		EBULLET,//�e��
	};
	//mTag�̒l��Ԃ�
	ETag GetTag();

	CCollider();
	~CCollider();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="position">�ʒu</param>
	/// <param name="radius">���a</param>
	/// <param name="tag">�^�O</param>
	CCollider(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, float radius, ETag tag = ETag::EBODY);
	void Render();

	//�D��x�̕ύX
	virtual void ChangePriority();
	//�D��x�̕ύX
	void ChangePriority(int priority);

	/// <summary>
	/// �R���C�_1��2�̏Փ˔���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	/// <returns>�Փ˂̉�</returns>
	static bool Collision(CCollider* m, CCollider* o);
	/// <summary>
	/// �O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
	/// </summary>
	/// <param name="triangle">�O�p�R���C�_</param>
	/// <param name="sphere">���R���C�_</param>
	/// <param name="adjust">�����l(�Փ˂��Ȃ��ʒu�܂Ŗ߂��l)</param>
	/// <returns>�Փ˂̉�</returns>
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere, CVector* adjust);
	/// <summary>
	/// �O�p�R���C�_�Ɛ����R���C�_�̏Փ˔���
	/// </summary>
	/// <param name="triangle">�O�p�R���C�_</param>
	/// <param name="line">�����R���C�_</param>
	/// <param name="adjust">�����l(�Փ˂��Ȃ��ʒu�܂Ŗ߂��l)</param>
	/// <returns>�Փ˂̉�</returns>
	static bool CollisionTriangleLine(CCollider* triangle, CCollider* line, CVector* adjust);
	/// <summary>
	/// �J�v�Z���R���C�_�ƃJ�v�Z���R���C�_�̏Փ˔���
	/// </summary>
	/// <param name="m">�J�v�Z���R���C�_</param>
	/// <param name="o">�J�v�Z���R���C�_</param>
	/// <param name="adjust">�����l</param>
	/// <returns>�Փ˂̉�</returns>
	static bool CollisionCapsuleCapseule(CCollider* m, CCollider* o, CVector* adjust);

	//�z��mV[i]�̗v�f��Ԃ�
	const CVector& GetV(int i);
	//�e�|�C���^�̎擾
	CCharacter3* GetParent();
	//�e�s��̐ݒ�
	void SetMatrix(CMatrix* m);
protected:
	EType mType;  //�R���C�_�^�C�v
	ETag mTag;    //�^�O
	CVector mV[3];//���_
	CCharacter3* mpParent;//�e
	CMatrix* mpMatrix;    //�e�s��
	float mRadius;//���a
};
#endif