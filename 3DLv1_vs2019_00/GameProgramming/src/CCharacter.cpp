#include "CCharacter.h"
#include "CTaskManager.h"

//�e�N�X�`���̐ݒ�
void CCharacter::Texture(CTexture* pTexture, int left, int right, int bottom, int top)
{
	mpTexture = pTexture;

	mLeft = left;

	mRight = right;

	mBottom = bottom;

	mTop = top;
}

//�`��
void CCharacter::Render()
{
	mpTexture->DrawImage(
		GetX() - GetW(),
		GetX() + GetW(),
		GetY() - GetH(),
		GetY() + GetH(),
		mLeft, mRight, mBottom, mTop
	);
}

//�f�t�H���g�R���X�g���N�^
CCharacter::CCharacter()
	: mpTexture(nullptr)
	, mLeft(0.0f), mRight(0.0f), mBottom(0.0f), mTop(0.0f)
{
	mPriority = 0;
	//CTaskManager::GetInstance()->Add(this);
}

//�R���X�g���N�^
CCharacter::CCharacter(int priority)
	: mpTexture(nullptr)
	, mLeft(0.0f), mRight(0.0f), mBottom(0.0f), mTop(0.0f)
{
	//�D�揇�ʂ̐ݒ�
	mPriority = priority;
	//�^�X�N�}�l�[�W���[�֒ǉ�
	//CTaskManager::GetInstance()->Add(this);
}

//�f�X�g���N�^
//�폜���ꂽ�烊�X�g������폜����
CCharacter::~CCharacter()
{
	//CTaskManager::GetInstance()->Remove(this);
}

//mpTexture��Ԃ�
CTexture* CCharacter::GetTexture()
{
	return mpTexture;
}

//�L���t���O��ݒ�
void CCharacter::SetEnabled(bool isEnabled)
{
	mEnabled = isEnabled;
}
//�L���t���O��Ԃ�
bool CCharacter::GetEnabled()
{
	return mEnabled;
}

CCharacter::ETag CCharacter::GetTag()
{
	return mTag;
}
