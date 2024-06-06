#ifndef CPALADIN_H
#define CPALADIN_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CWepon.h"

class CPaladin : public CXCharacter
{
public:
	CPaladin();
	//����̍X�V
	void WeponUpdate();
	//����̕`��
	void WeponRender();
private:
	static CModelX sModel;
	CWepon mWepon;
};
#endif