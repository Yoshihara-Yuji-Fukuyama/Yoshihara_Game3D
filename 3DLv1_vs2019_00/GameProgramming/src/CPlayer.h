#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer :public CCharacter3
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();
	void Collision();
	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
	CPlayer();
	//CPlayer(�ʒu,��],�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//�X�V����
	void Update();
private:
	//�v���C���[�̃C���X�^���X
	static CPlayer* spInstance;
	CInput mInput;
	CColliderLine mLine;//�����R���C�_
	CColliderLine mLine2;
	CColliderLine mLine3;
};
#endif