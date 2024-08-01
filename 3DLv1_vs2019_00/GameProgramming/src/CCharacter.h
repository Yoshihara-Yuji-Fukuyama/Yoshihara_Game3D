#ifndef CCHARACTER_H
#define CCHARACTER_H

#include "CTexture.h"
#include "CRectangle.h"
#include "CTask.h"

//�R���C�_�N���X�̐錾
class CCollider;

class CCharacter :public CRectangle, public CTask//�e�N���X��
{
public:

	enum class ETag //���ʎq
	{
		EPLAYER,	//�v���C���[
		ESLIME,		//�X���C��
		EONI,			//�S
	};

	enum class EState//���
	{
		EWAIT,    //�ҋ@
		EMOVE,    //�ړ�
		EJUMP,    //�W�����v
		EATTACK,	  //�U��
		EHIT,     //�G�̍U������������
		GUARD,	  //�h�� 
	};
	//�f�t�H���g�R���X�g���N�^
	CCharacter();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="priority">�D��x</param>
	CCharacter(int priority);

	//�f�X�g���N�^
	virtual ~CCharacter();

	//mpTexture��Ԃ�
	CTexture* GetTexture();

	/// <summary>
	/// �L���t���O��ݒ�
	/// </summary>
	/// <param name="isEnabled"></param>
	void SetEnabled(bool isEnabled);

	//�L���t���O��Ԃ�
	bool GetEnabled();

	/// <summary>
	/// �������Ԃ�ݒ�
	/// </summary>
	/// <param name="order">��������</param>
	void SetSortOrder(float order);

	//�������Ԃ��擾
	float GetSortOrder();

	/// <summary>
	/// �e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="pTexture">�e�N�X�`���̃|�C���^</param>
	/// <param name="left">�e�N�X�`���̍����W</param>
	/// <param name="right">�e�N�X�`���̉E���W</param>
	/// <param name="bottom">�e�N�X�`���̉����W</param>
	/// <param name="top">�e�N�X�`���̏���W</param>
	void Texture(CTexture* pTexture, int left, int right, int bottom, int top);

	void Render();//�`��

		//�Փˏ���2
	virtual void Collision() {};
	//�Փˏ���4
	virtual void Collision(CCharacter* m, CCharacter* o) {};

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	virtual void Collision(CCollider* m, CCollider* o) {}

	//�����̍��W���擾
	float GetUnderPosY();
	//�e�̍��W���擾
	float GetShadowPosY();

	//HP���擾
	float GetHp();
	//HP��ݒ�
	void SetHp(float hp);

	//���ʎq�̎擾
	ETag GetTag();

	EState GetState();

	void SetLeg(float leg);

protected:
	ETag mTag;			//���ʎq�̊i�[
	EState mState;      //���
	bool isMove;        //�ړ����Ă��邩
	bool isMoveX;       //X���ړ����Ă��邩
	bool isMoveY;       //Y���ړ����Ă��邩
	bool isAttack;      //�U�����Ă��邩
	bool isAttackNext;  //�����U�����邩
	bool isHit;			//�U�����󂯂Ă��邩
	bool isGuard;		//�h�䂵�Ă��邩
	bool isGeneration;	//�R���C�_����������Ă��邩
	//�����v�Z�p
	float mLeg;
	//�e�v�Z�p
	float mShadow;
	//�����̍��W
	float mUnderPosY;
	//�e�̍��W
	float mShadowPosX;
	float mShadowPosY;
	//�W�����v����
	float mJump;
	//HP
	float mHp;
	//�A�j���[�V�������Ƃ̖���
	int MoveNum;
	int AttackNum;
	int AttackNum2;
	int AttackNum3;
	int WaitNum;
	int JumpNum;
	int HitNum;
	int GuardNum;

	float mVx, mVy;		//���x
private:
	CTexture* mpTexture;//�e�N�X�`��


	int mLeft, mRight, mBottom, mTop;//���A�E�A���A��

};
#endif