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
#include "CPaladin.h"
#include "CActionCamera.h"

#include "CTransform.h"
#include "CMatrix.h"
#include "CVector.h"
#include <vector>

#include "CInput.h"
#include "CFont.h"
#include "CUi.h"
#include "CSound.h"

#include "CWepon.h"
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

	void Start();
	void Update();

	void SpawnEnemy();
private:
	static CMatrix mModelViewInverse;//���f���r���[�̋t�s��
	static CUi* spUi;                //Ui�N���X�̃|�C���^
	static CTexture mTexture;        //Texture�N���X�̃C���X�^���X
	CModel mModel;
	CModel mModelC5;
	CModel mBackGround;//�w�i�̃��f��
	CModelX mPlayerModel; //�v���C���[�̃��f��

	CColliderTriangle mColliderTriangle;
	CColliderMesh mColliderMesh;//���f������R���C�_�𐶐�
	CMatrix mMatrix;

	CXPlayer mXPlayer;
	std::vector<CXEnemy*> mpXEnemy;

	CEnemy3* mpEnemy3;

	CPlayer* mpPlayer;
	CEnemy* mpEnemy;
	CPaladin* mpPaladin;

	CInput mInput;
	CFont mFont;

	CVector mEye; //�J�����̎��_
	CActionCamera mActionCamera;//CActionCamera

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

	CWepon* mWepon;
};