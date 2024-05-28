#ifndef CPALADIN_H
#define CPALADIN_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CColliderCapsule.h"
#include "CInput.h"

class CPaladin : public CXCharacter
{
public:
	CPaladin();
private:
	static CModelX sModel;
};
#endif