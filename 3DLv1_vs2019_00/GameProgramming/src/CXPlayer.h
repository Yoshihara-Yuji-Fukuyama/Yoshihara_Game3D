#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"

class CXPlayer : public CXCharacter{
public:
	void Update();
private:
	CInput mInput;//CInput�̃C���X�^���X�쐬
};
#endif