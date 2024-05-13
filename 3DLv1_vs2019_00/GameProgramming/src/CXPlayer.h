#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
public:
	CXPlayer();
	void Update();
	//�����ݒ�
	//���R���C�_�̐����ƃ{�[���Ƃ̘A��
	void Init(CModelX* model);
private:
	CCollider mColSphereHead; //��
	CCollider mColSphereBody; //��
	CCollider mColSphereSword;//��
	CInput mInput;//CInput�̃C���X�^���X�쐬
};
#endif