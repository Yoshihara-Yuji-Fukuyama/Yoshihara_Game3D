#pragma once
#include "CModel.h"
#include "CModelX.h"
#include "CXCharacter.h"
#include "CCharacter3.h"
#include "CTexture.h"
#include "CTriangle.h"
#include "CBillBoard.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CTaskManager.h"

#include "CPlayer.h"
#include "CEnemy.h"
#include "CEnemy3.h"
#include "CBullet.h"
#include "CXPlayer.h"
#include "CXEnemy.h"

#include "CTransform.h"
#include "CMatrix.h"
#include "CVector.h"
#include <vector>

#include "CInput.h"
#include "CFont.h"
#include "CUi.h"
#include "CSound.h"

class CApplication
{
public:
	//���f���r���[�s��̎擾
	static const CMatrix& GetModelViewInverse();
	//Texture�N���X�̃C���X�^���X���擾
	static CTexture* GetTexture();
	//UI�N���X�̃C���X�^���X���擾
	static CUi* GetUi();
	//�f�X�g���N�^
	~CApplication();
	//�����ݒ�
	void Init();

	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();

private:
	static CMatrix mModelViewInverse;//���f���r���[�̋t�s��
	static CUi* spUi;                //Ui�N���X�̃|�C���^
	static CTexture mTexture;        //Texture�N���X�̃C���X�^���X
	CModel mModel;
	CModel mModelC5;
	CModel mBackGround;
	CModelX mPlayerModel; //�v���C���[�̃��f��
	CModelX mKnightModel; //Knight�̃��f��

	CColliderMesh mColliderMesh;
	CMatrix mMatrix;

	CXPlayer mXPlayer;
	CXEnemy mXEnemy;

	CPlayer* mpPlayer;
	CEnemy* mpEnemy;

	CInput mInput;
	CFont mFont;

	CVector mEye; //�J�����̎��_

	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[��

	//�Q�[���̏��
	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
};