#ifndef CVERTEX_H
#define CVERTEX_H
#include "CVector.h"
/*
���_�f�[�^�N���X
���_���ƂɃf�[�^���܂Ƃ߂܂�
*/
class CVertex
{
public:
	//�ʒu
	CVector mPosition;
	//�@��
	CVector mNormal;
	//�e�N�X�`���}�b�s���O
	CVector mTextureCoords;
};
#endif